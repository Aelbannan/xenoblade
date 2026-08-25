// TU for kyoshin/CPcKizunagram - PC affinity chart window.
// func_8025DA40 / func_8025DA48: FULL_MATCH (byte-identical).

#include "kyoshin/CPcKizunagram.hpp"

#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"

// Forward declarations of intra-TU callees (free unmangled functions).
void func_8025E0D8(CPcKizunagram* self);
void func_8025D688(CPcKizunaCur* self);
extern "C" CPcKizunaSlotEntry* func_8025F290(CPcKizunaSlotEntry* p);

// Layout animation helpers from code_80135FDC (retail unmangled func_80137510).
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

// String base for the affinity-chart layout paths (targets add byte offsets).
extern char lbl_eu_8050D868[];

// Column-name format base (func_8025EE94 uses +0x06).
extern char lbl_eu_8050DB18[];

extern "C" void func_8025F114(CPcKizunagramBig* self, CPcKizunaSlotEntry* entry);

// Named float constants in .sdata2 referenced via @sda21 by func_8025E56C/E5A8.
extern const float lbl_eu_8066887C;
extern const float lbl_eu_80668880;
extern const float lbl_eu_80668890;
extern const float lbl_eu_80668894;
extern const float lbl_eu_80668878;

// Layout / file helpers with C++ (mangled) retail names.
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
void func_801390E0(CFileHandle**);
void func_80139124(nw4r::lyt::ArcResourceAccessor*);

u8 func_8025DA40(CPcKizunagram* pKizunagram) { return pKizunagram->mIsHidden; }










extern "C" void __declspec(noinline) func_8025D704(CPcKizunaCur* self) {
    if (func_80137444(self->mpAnim1, lbl_eu_8066887C) == 0) return;
    self->mField14 = 0;
    self->mField15 = 1;
    self->mpLayout->SetAnimationEnable(self->mpAnim1, false);
    self->mpLayout->SetAnimationEnable(self->mpAnim0, true);
    self->mpAnim0->SetFrame(lbl_eu_80668880);
}

// Step the current sub-state of the cursor, then always re-animate its layout.
extern "C" void __declspec(noinline) func_8025D610(CPcKizunaCur* self) {
    if (self->mpLayout == 0) return;
    switch (self->mField14) {
    case 0:
        func_80137444(self->mpAnim0, lbl_eu_8066887C);
        break;
    case 1:
        func_8025D704(self);
        break;
    }
    self->mpLayout->Animate(0);
}

extern "C" void func_8025D8C4(CPcKizunagram* self) {
    if (self->mStateByte1 == 0) return;
    if (self->mStateByte2 == 0) return;
    switch (self->mStateByte2) {
    case 1:
        func_8025DC08(self);
        break;
    case 2:
        func_8025DC8C(self);
        break;
    case 3:
        func_8025DCB0(self);
        break;
    }
    self->mLayout->Animate(0);
    func_8025D610((CPcKizunaCur*)self->mKizunaCur);
}

// Destroy the cursor's layout: delete it through its vtable (deleting dtor,
// slot 2) and null out the pointer. The double null-check is MWCC's `delete`
// lowering for this class (same shape as func_8025D9C4's mLayout delete).
extern "C" void __declspec(noinline) func_8025D688(CPcKizunaCur* self) {
    if (self->mpLayout != 0) {
        delete self->mpLayout;
        self->mpLayout = 0;
    }
}

// Draw the main layout and, if present, the embedded cursor layout.
void func_8025D954(CPcKizunagram* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->mStateByte1 == 0) return;
    if (self->mStateByte2 == 0) return;
    func_80137038(self->mLayout, drawInfo, 0, 1);
    nw4r::lyt::Layout* cursorLayout = ((CPcKizunaCur*)self->mKizunaCur)->mpLayout;
    if (cursorLayout != 0) {
        func_80137038(cursorLayout, drawInfo, 0, 1);
    }
}

extern "C" void func_8025D9C4(CPcKizunagram* self) {
    func_801390E0(&self->mFileHandle);
    self->mStateByte1 = 0;
    func_8025D688((CPcKizunaCur*)self->mKizunaCur);
    if (self->mLayout != 0) {
        delete self->mLayout;
        self->mLayout = 0;
    }
    func_80139124(self->mArcRes);
    self->mMemRegion.func_8045F778();
}


u8 func_8025DA48(CPcKizunagram* pKizunagram) { return pKizunagram->mIsOpen; }

// CPcKizunaCur constructor: paint the fixed layout with the vtable and the
// embedded accessor, then zero the following fields (visible flag = 1).
// C-linkage (retail symbol __ct__CPcKizunaCur is unmangled) and noinline so
// retail's __ct__CPcKizunagram call stays a direct bl.
extern "C" void __declspec(noinline) __ct__CPcKizunaCur(CPcKizunaCur* self,
                                                         nw4r::lyt::ArcResourceAccessor* accessor) {
    self->mVtable = lbl_eu_805376AC;
    self->mAccessor = accessor;
    self->mpLayout = 0;
    self->mpAnim0 = 0;
    self->mpAnim1 = 0;
    self->mField14 = 0;
    self->mField15 = 1;
    self->mField16 = 0;
}

CPcKizunaCur::~CPcKizunaCur() {}

// CPcKizunagram destructor. MWCC auto-generates the member destruction
// (mMemRegion has a non-trivial dtor; the cursor's is trivial and elided) and
// the delete-flag handling (r4 > 0 -> operator delete) for the
// complete-object destructor.
CPcKizunagram::~CPcKizunagram() {}

// CPcKizunagram constructor. Base class stores the manual vtable pointer,
// then the embedded UnkClass_8045F564 is default-constructed, then the
// remaining fields and the placeholder-constructed cursor are initialised.
CPcKizunagram::CPcKizunagram() {
    mFileHandle = 0;
    mArcRes = 0;
    mLayout = 0;
    mAnimTransform = 0;
    mStateByte1 = 0;
    mStateByte2 = 0;
    mIsHidden = 0;
    mIsOpen = 1;
    mField28 = 0;
    __ct__CPcKizunaCur(reinterpret_cast<CPcKizunaCur*>(mKizunaCur), 0);
    mByte44 = 0;
    mFloat48 = lbl_eu_80668880;
}

extern "C" void func_8025D6E0(CPcKizunaTreeRoot* self, CPcKizunaVec3* src) {
    CPcKizunaTreeLeaf* leaf = self->field8->field10;
    leaf->x = src->x;
    leaf->y = src->y;
    leaf->z = src->z;
}

void func_8025DA50(CPcKizunagram* self) {
    if (self->mStateByte2 != 0) return;
    self->mStateByte2 = 1;
    self->mIsOpen = 0;
    func_80138078__FUl(0x6d);
}

// Close the affinity-chart window: from the open state (2) go to closing (3),
// hide the cursor pane and play the close sound.
void func_8025DA78(CPcKizunagram* self) {
    if (self->mStateByte2 != 2) return;
    self->mStateByte2 = 3;
    self->mIsOpen = 0;
    func_80124270(((CPcKizunaCur*)self->mKizunaCur)->mpLayout->GetRootPane()
                      ->FindPaneByName(lbl_eu_8050D868 + 0x50, true),
                  0);
    func_80138078__FUl(6);
}

void func_8025DAE8(CPcKizunagram* self) {
    func_8025E3A4(self, 1);
    func_8025E4A4(self);
    func_8025DCFC(self);
    func_80138078__FUl(1);
}

void func_8025DB30(CPcKizunagram* self) {
    func_8025E3A4(self, 0);
    func_8025E4A4(self);
    func_8025DCFC(self);
    func_80138078__FUl(1);
}

void func_8025DB78(CPcKizunagram* self) {
    func_8025E3A4(self, 1);
    func_8025E4A4(self);
    func_8025DCFC(self);
    func_80138078__FUl(1);
}

void func_8025DBC0(CPcKizunagram* self) {
    func_8025E3A4(self, 0);
    func_8025E4A4(self);
    func_8025DCFC(self);
    func_80138078__FUl(1);
}

extern "C" void __declspec(noinline) func_8025DC08(CPcKizunagram* self) {
    if (func_80137444(self->mAnimTransform, lbl_eu_8066887C) == 0) return;
    self->mStateByte2 = 2;
    self->mIsOpen = 1;
    func_8025E4A4(self);
    func_80124270(((CPcKizunaCur*)self->mKizunaCur)->mpLayout->GetRootPane()
                      ->FindPaneByName(lbl_eu_8050D868 + 0x50, true),
                  1);
}

// Dispatch on the state byte at +0x44; each branch is a tail call.
extern "C" void __declspec(noinline) func_8025DC8C(CPcKizunagram* self) {
    switch (self->mByte44) {
    case 0:
        func_8025E56C(self);
        break;
    case 1:
        func_8025E5A8(self);
        break;
    }
}

// Start an async file read of the affinity-chart layout; stash the handle and
// mark the file job for flag-2 handling.
void func_8025D874(CPcKizunagram* self) {
    self->mFileHandle = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), lbl_eu_8050D868 + 0x66, (IWorkEvent*)self, 0, 0);
    CDeviceFile::setHandleFlag2(self->mFileHandle);
}

// When the opening animation (mAnimTransform at +0x20) finishes, mark the
// window as open (sub-state 0, mIsOpen = 1).
extern "C" void __declspec(noinline) func_8025DCB0(CPcKizunagram* self) {
    if (func_80137510(self->mAnimTransform, lbl_eu_8066887C) != 0) {
        self->mStateByte2 = 0;
        self->mIsOpen = 1;
    }
}

// These three are large retail functions (0x100/0xc8/0x3dc) not yet
// implemented here; noinline keeps callers emitting real bl instructions, and
// extern "C" keeps the unmangled retail symbol names.
// func_8025DCFC: refresh all 6 affinity rows for the current character.
extern "C" __declspec(noinline) void func_8025DCFC(CPcKizunagram* self) {
    const void* table = getFP__FPCc(lbl_eu_8050D868 + 0x7f);
    u8 count = (u8)func_8003B1EC((void*)table);
    func_8009CF8C(0x20);

    for (u8 n = 1; n <= count; n++) {
        char* paneName = func_8013639C(table, lbl_eu_8050D868 + 0x90, (u8)n);
        nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(paneName, true);
        if (func_8025E960(self, table, (u8)n) != 0) {
            // --- selected row ---
            if (pane != 0) func_80124270(pane, 1);
            if (func_8025E9E4(self, table, (u8)n) != 0) {
                func_80137C1C(pane, 0x7777ffff);
            }
            for (u32 k = 1; k <= 6; k++) {
                char buf[0x20];
                sprintf(buf, lbl_eu_8050D868 + 0x96, (u8)k);
                u32 e8 = func_801361E8((u32)table, buf, (u8)n);
                int rnd = (e8 & 0xff) + 0x29;
                func_8009CF8C(rnd);
                u8 lb8 = 0, lb9 = 0;
                func_8013AB0C(&lb9, &lb8, rnd);
                int r21 = 0;
                if (func_8025E904(self, table, lb9) != 0 && func_8025E904(self, table, lb8) != 0) {
                    r21 = 1;
                }
                int id = (e8 & 0xff) + 1;
                char* str = (char*)func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xaf, id);
                nw4r::lyt::Pane* p = self->mLayout->GetRootPane()->FindPaneByName(str, true);
                if (p != 0) func_80124270(p, r21);
                if (r21 != 0) {
                    u32 tex = (u32)self->mArcRes->GetResource(0x74696d67, lbl_eu_8050D868 + 0xb5, 0);
                    if (tex != 0) func_80137F88(p, tex);
                }
                char* str2 = (char*)func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xca, id);
                nw4r::lyt::Pane* p2 = self->mLayout->GetRootPane()->FindPaneByName(str2, true);
                if (p2 != 0) func_80124270(p2, 0);
                char* str3 = (char*)func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xd4, id);
                nw4r::lyt::Pane* p3 = self->mLayout->GetRootPane()->FindPaneByName(str3, true);
                if (p3 != 0) func_80124270(p3, 0);
            }
        } else {
            // --- deselected row ---
            if (pane != 0) func_80124270(pane, 0);
            if ((u8)n == 5) {
                u16 r25 = (u16)func_8013606C(lbl_eu_8050D868 + 0x7f, lbl_eu_8050D868 + 0xdd, 5);
                if (func_8009CF8C(0x20) >= r25) {
                    for (u32 k = 1; k <= 6; k++) {
                        char buf[0x20];
                        sprintf(buf, lbl_eu_8050D868 + 0x96, (u8)k);
                        u32 e8 = func_801361E8((u32)table, buf, (u8)n);
                        int id = (e8 & 0xff) + 1;
                        char* str = (char*)func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xaf, id);
                        nw4r::lyt::Pane* p = self->mLayout->GetRootPane()->FindPaneByName(str, true);
                        if (p != 0) func_80124270(p, 0);
                        char* str2 = (char*)func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xca, id);
                        nw4r::lyt::Pane* p2 = self->mLayout->GetRootPane()->FindPaneByName(str2, true);
                        if (p2 != 0) func_80124270(p2, 0);
                        char* str3 = (char*)func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xd4, id);
                        nw4r::lyt::Pane* p3 = self->mLayout->GetRootPane()->FindPaneByName(str3, true);
                        if (p3 != 0) func_80124270(p3, 0);
                    }
                }
            }
        }
    }
    func_8025E0D8(self);
}

void func_8025E0D8(CPcKizunagram* self) {
    const void* table = getFP__FPCc(lbl_eu_8050D868 + 0x7f);
    // First pass: build the character-row lookup table (BDAT indexed by row).
    // u8 counter/count drive the retail clrlwi + cmplw (unsigned) loop.
    u8 count = (u8)func_8003B1EC((void*)table);
    for (u8 i = 0; i < count; i++) {
        func_8013639C(table, lbl_eu_8050D868 + 0x90, (u8)i + 1);
    }

    // Second pass: refresh the 6 affinity rows (u32 counter -> cmplwi/ble).
    for (u32 k = 1; k <= 6; k++) {
        char buf[0x20];
        sprintf(buf, lbl_eu_8050D868 + 0x96, (u8)k);
        u32 e8 = func_801361E8((u32)table, buf, (s8)self->mField28 + 1);
        int id = (e8 & 0xff) + 1;
        u16 val = (u16)func_8009CF8C((e8 & 0xff) + 0x29);

        char* str1 = (char*)func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xaf, id);
        nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(str1, true);
        if (pane == 0) continue;
        if (func_801C4648() == 0) continue;

        // Select a resource name for the mood/face texture by affinity value.
        const char* s1;
        if (val < 0xbb8) {
            if (val < 0x7d0) {
                s1 = (val < 0x3e8) ? lbl_eu_8050D868 + 0xe4 : lbl_eu_8050D868 + 0xf9;
            } else {
                s1 = lbl_eu_8050D868 + 0x10e;
            }
        } else {
            s1 = (val >= 0x2711) ? 0 : (val >= 0x1388) ? lbl_eu_8050D868 + 0x138 : lbl_eu_8050D868 + 0x123;
        }
        u32 tex1 = (u32)self->mArcRes->GetResource(0x74696d67, s1, 0);
        if (tex1 != 0) func_80137F88(pane, tex1);

        char* str2 = (char*)func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xca, id);
        const char* s2;
        if (val >= 0xbb8) {
            s2 = (val >= 0x2711) ? 0 : (val >= 0x1388) ? lbl_eu_8050D868 + 0x1ad : lbl_eu_8050D868 + 0x195;
        } else if (val >= 0x7d0) {
            s2 = lbl_eu_8050D868 + 0x17d;
        } else {
            s2 = (val >= 0x3e8) ? lbl_eu_8050D868 + 0x165 : lbl_eu_8050D868 + 0x14d;
        }
        u32 tex2 = (u32)self->mArcRes->GetResource(0x74696d67, s2, 0);
        if (tex2 != 0) {
            nw4r::lyt::Pane* pane2 = self->mLayout->GetRootPane()->FindPaneByName(str2, true);
            func_80137F88(pane2, tex2);
            if (pane2 != 0) func_80124270(pane2, 1);
        }

        char* str3 = (char*)func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xd4, id);
        func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->mLayout, str3, val);
        nw4r::lyt::Pane* pane3 = self->mLayout->GetRootPane()->FindPaneByName(str3, true);
        if (pane3 != 0) func_80124270(pane3, 1);
    }
}


// Initialise the per-character cursor widget: load its layout and two animation
// transforms, enable the idle anim, pin down two named panes, then reset the
// root pane scale. noinline: retail callers (OnFileEvent) emit a real bl.
extern "C" void __declspec(noinline) func_8025D4E4(CPcKizunaCur* self) {
    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        &self->mpLayout, self->mAccessor, lbl_eu_8050D868 + 0x00);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        self->mpLayout, &self->mpAnim0, self->mAccessor, lbl_eu_8050D868 + 0x18);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        self->mpLayout, &self->mpAnim1, self->mAccessor, lbl_eu_8050D868 + 0x35);
    self->mpLayout->SetAnimationEnable(self->mpAnim1, false);
    self->mpLayout->SetAnimationEnable(self->mpAnim0, true);
    self->mpLayout->Animate(0);
    nw4r::lyt::Pane* p1 = self->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_8050D868 + 0x50, true);
    func_80124270(p1, 0);
    nw4r::lyt::Pane* p2 = self->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_8050D868 + 0x5b, true);
    func_80124270(p2, 0);
    self->mpLayout->GetRootPane()->SetScale(nw4r::math::VEC2(lbl_eu_80668878, lbl_eu_80668878));
}

// Move the affinity-chart cursor by one row in the given direction (arg!=0
// steps backward, wrapping 0->7), skipping invalid target rows.
extern "C" void func_8025E3A4(CPcKizunagram* self, u32 arg) {
    // 8-entry row order table built from two sdata words.
    u32 order[2];
    order[0] = lbl_eu_80668888;
    order[1] = lbl_eu_8066888C;
    func_8003AA34();
    void* table = getFP__FPCc(lbl_eu_8050D868 + 0x7f);
    int count = func_8003B1EC(table);
    // Locate the current character row inside the 8-entry order list.
    // Constant-bound for-loop lets MWCC drive the trip count with mtctr/bdnz
    // while keeping the row index in a register like retail (r6).
    int idx = 0;
    for (u8 k = 0; k < 8; k++) {
        u8 b = ((u8*)order)[k];
        if (b == (s8)self->mField28) {
            idx = k;
            break;
        }
    }
    // Step the cursor and commit the first BDAT-valid target row found.
    // Single index variable: retail reuses one register (r26) for both the
    // search result and the stepping cursor.
    int tries = 0;
    do {
        if (arg != 0) {
            idx = idx - 1;
            if (idx < 0) idx = 7;
        } else {
            idx = idx + 1;
            if (idx > 7) idx = 0;
        }
        if (func_8025E960(self, table, (u8)(((u8*)order)[idx] + 1)) != 0) {
            self->mField28 = ((u8*)order)[idx];
            break;
        }
        tries++;
    } while (tries <= count);
}

// Target a text pane by name, then move the cursor so it sits over the row
void __declspec(noinline) func_8025E4A4(CPcKizunagram* self) {
    char* path =
        (char*)func_80136190(lbl_eu_8050D868 + 0x7f, lbl_eu_8050D868 + 0x90, (s8)self->mField28 + 1);
    nw4r::lyt::Pane* pane1 = self->mLayout->GetRootPane()->FindPaneByName(path, true);
    nw4r::lyt::Pane* pane2 =
        self->mLayout->GetRootPane()->FindPaneByName(lbl_eu_8050D868 + 0x1c5, true);
    CPcKizunaVec3 pos;
    CPcKizunaVec3 tmp;
    func_80137924(&pos, pane1, pane2, self->mLayout->GetRootPane());
    tmp.x = pos.x;
    tmp.y = pos.y;
    tmp.z = pos.z;
    func_8025D6E0((CPcKizunaTreeRoot*)self->mKizunaCur, &tmp);
}

extern "C" void __declspec(noinline) func_8025E56C(CPcKizunagram* self) {
    self->mFloat48 += lbl_eu_8066887C;
    if (self->mFloat48 >= lbl_eu_80668890) {
        self->mByte44 = 1;
        self->mFloat48 = lbl_eu_80668880;
        func_8025E5E4(self, 1);
    }
}

extern "C" void __declspec(noinline) func_8025E5A8(CPcKizunagram* self) {
    self->mFloat48 += lbl_eu_8066887C;
    if (self->mFloat48 >= lbl_eu_80668894) {
        self->mByte44 = 0;
        self->mFloat48 = lbl_eu_80668880;
        func_8025E5E4(self, 0);
    }
}

extern "C" void func_8025E5E4(CPcKizunagram* self, u32 value) {
    const void* table = getFP__FPCc(lbl_eu_8050D868 + 0x7f);
    u8 count = (u8)func_8003B1EC((void*)table);
    for (u8 i = 0; i < count; i++) {
        func_8013639C(table, lbl_eu_8050D868 + 0x90, (u8)i + 1);
    }

    for (u32 k = 1; k <= 6; k++) {
        char buf1[0x20];
        sprintf(buf1, lbl_eu_8050D868 + 0x96, (u8)k);
        u32 e8 = func_801361E8((u32)table, buf1, (s8)self->mField28 + 1);
        int id = (e8 & 0xff) + 1;
        u16 val = (u16)func_8009CF8C((e8 & 0xff) + 0x29);

        char* str1 = (char*)func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xaf, id);
        nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(str1, true);
        if (pane == 0) continue;
        if (func_801C4648() == 0) continue;

        // Select a fixed resource-name for the mood texture by affinity value.
        const char* s1;
        if (val < 0xbb8) {
            s1 = (val < 0x7d0) ? (val < 0x3e8) ? lbl_eu_8050D868 + 0xe4 : lbl_eu_8050D868 + 0xf9
                                : lbl_eu_8050D868 + 0x10e;
        } else {
            s1 = (val >= 0x2711) ? 0 : (val >= 0x1388) ? lbl_eu_8050D868 + 0x138 : lbl_eu_8050D868 + 0x123;
        }
        u32 tex1 = (u32)self->mArcRes->GetResource(0x74696d67, s1, 0);
        if (tex1 != 0) func_80137F88(pane, tex1);

        // Second panel: build its resource name from a value-dependent format.
        // Each range chooses a distinct format string; the val>=0x2711 range
        // skips the sprintf entirely (falls straight to the resource fetch).
        char* str2 = (char*)func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xca, id);
        char buf2[0x20];
        if (val < 0xbb8) {
            if (val < 0x7d0) {
                if (val < 0x3e8)
                    sprintf(buf2, lbl_eu_8050D868 + 0x1d4, value);
                else
                    sprintf(buf2, lbl_eu_8050D868 + 0x1ee, value);
            } else {
                sprintf(buf2, lbl_eu_8050D868 + 0x208, value);
            }
        } else if (val < 0x2711) {
            if (val < 0x1388)
                sprintf(buf2, lbl_eu_8050D868 + 0x222, value);
            else
                sprintf(buf2, lbl_eu_8050D868 + 0x23c, value);
        }
        u32 tex2 = (u32)self->mArcRes->GetResource(0x74696d67, buf2, 0);
        if (tex2 != 0) {
            nw4r::lyt::Pane* pane2 = self->mLayout->GetRootPane()->FindPaneByName(str2, true);
            func_80137F88(pane2, tex2);
            if (pane2 != 0) func_80124270(pane2, 1);
        }

        char* str3 = (char*)func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xd4, id);
        func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->mLayout, str3, (u8)val);
        nw4r::lyt::Pane* pane3 = self->mLayout->GetRootPane()->FindPaneByName(str3, true);
        if (pane3 != 0) func_80124270(pane3, 1);
    }
}

extern "C" int func_8025E904(CPcKizunagram* self, const void* table, int val) {
    // Map the input via two byte tables built from sdata constants, then score
    // the mapped row through the standard BDAT range check.
    u8 t1[8];
    *(u32*)&t1[0] = lbl_eu_80668898;
    *(u32*)&t1[4] = lbl_eu_8066889C;
    u8 t2[8];
    *(u32*)&t2[0] = lbl_eu_806688A0;
    *(u32*)&t2[4] = lbl_eu_806688A4;
    u8 idx = t1[val - 1];
    return func_8025E960(self, table, (u8)(t2[idx] + 1));
}

// BDAT range check: random row must fall between two bounded column values.
// noinline: retail callers emit a real bl (func_8025E904 etc).
extern "C" int __declspec(noinline) func_8025E960(CPcKizunagram* self, const void* table, int id) {
    const char* base = lbl_eu_8050D868;
    u16 v1 = func_80136254(table, base + 0xdd, id);
    u16 v2 = func_80136254(table, base + 0x256, id);
    u16 check = (u16)func_8009CF8C(0x20);
    int result = 0;
    if ((u32)v1 <= (u32)check && (u32)check <= (u32)v2) result = 1;
    return result;
}

extern "C" int func_8025E9E4(CPcKizunagram* self, const void* table, int id) {
    u16 v1 = func_80136254(table, lbl_eu_8050D868 + 0x25d, id);
    u16 v2 = func_80136254(table, lbl_eu_8050D868 + 0x264, id);
    u16 check = (u16)func_8009CF8C(0x20);
    int result = 0;
    if ((u32)v1 <= (u32)check && (u32)check <= (u32)v2) result = 1;
    return result;
}

// File-load completion callback: build the affinity-chart layout from the
// freshly-loaded arc, bind the font, construct the embedded cursor, and mark
// the window ready for use.
int CPcKizunagram::OnFileEvent(CEventFile* event) {
    if (mFileHandle == event->mFileHandle) {
        CPcKizunagram* self = this;
        u8 regionBuf[8];
        char* const s = lbl_eu_8050D868;
        self->mMemRegion.createRegion(mtl::MemManager::getHandleMEM2(), 0x12000, &s[0x26b], 0);
        __ct__14Class_8045F858FP17UnkClass_8045F564((Class_8045F858*)regionBuf, &self->mMemRegion);

        void* fileData = self->mFileHandle->getData();
        mtl::MemManager::func_80434A4C(false);
        self->mArcRes = createArcResourceAccessor__10CLibLayoutFv();
        self->mArcRes->Attach(fileData, &s[0x279]);
        func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
            &self->mLayout, self->mArcRes, &s[0x27d]);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            self->mLayout, &self->mAnimTransform, self->mArcRes, &s[0x293]);

        // Bind the font handle into the layout's root pane.
        nw4r::lyt::Pane* rootPane = self->mLayout->GetRootPane();
        CDeviceFontVtblView* font =
            (CDeviceFontVtblView*)func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, self->mLayout);
        u32 fontResult = font->vf7();
        func_8013676C(rootPane, fontResult);
        func_801355BC();

        self->mLayout->SetAnimationEnable(self->mAnimTransform, true);
        self->mLayout->Animate(0);

        // Build the cursor on the stack, copy its body into the member
        // region (skipping the +0x00 vtable pointer), then initialise it.
        u8 tmpCur[0x18];
        __ct__CPcKizunaCur((CPcKizunaCur*)tmpCur, self->mArcRes);
        CPcKizunaCur* curDst = (CPcKizunaCur*)self->mKizunaCur;
        CPcKizunaCur* curSrc = (CPcKizunaCur*)tmpCur;
        curDst->mAccessor = curSrc->mAccessor;
        curDst->mpLayout = curSrc->mpLayout;
        curDst->mpAnim0 = curSrc->mpAnim0;
        curDst->mpAnim1 = curSrc->mpAnim1;
        curDst->mField14 = curSrc->mField14;
        curDst->mField15 = curSrc->mField15;
        curDst->mField16 = curSrc->mField16;
        func_8025D4E4(curDst);

        self->mIsHidden = 1;
        self->mStateByte1 = 1;
        func_8025DCFC(self);
        self->mFileHandle = 0;
        self->mMemRegion.func_8045F810();
        __dt__14Class_8045F858Fv((Class_8045F858*)regionBuf, -1);
        return 1;
    }
    return 0;
}

// Compress an 11-slot CPcKizunagramBig into the compact CPcKizunaCompact:
// each 0xC4 slot collapses to 0xD bytes (the set state bytes only).
// Retail saves r30/r31 with individual stw's (speed-style prologue) even
// though the unit is -O4,s; disable lmw/stmw for this function only.
#pragma push
#pragma use_lmw_stmw off
// src is non-const: a const param makes MWCC hoist the tail-field loads
// above the LR-save/prologue stores, which retail does not do.
extern "C" void func_8025EC0C(CPcKizunaCompact* dst, CPcKizunagramBig* src) {
    dst->field_0xA8 = src->field_0x89C;
    dst->field_0x90 = src->field_0x884;
    memcpy(dst->data94, src->data888, 0x14);
    // The nonvolatiles hold the parameters themselves (retail r31=src,
    // r30=dst); the slot cursor aliases src and the entry cursor is a copy
    // taken after the call.
    const CPcKizunaSlot* s = &src->slots[0];
    const CPcKizunaSlot* e = s;
    CPcKizunaCompactSlot* d = &dst->slots[0];
    for (int i = 0; i < 0xb; i++) {
        d->bytes[0xC] = s->byteC0;
        d->bytes[0] = (u8)e->data00.word;
        d->bytes[1] = e->data00.byte14;
        // Advance the entry cursor right after its last use.
        e++;
        d->bytes[2] = (u8)s->sub[0].word;
        d->bytes[3] = s->sub[0].byte14;
        d->bytes[4] = (u8)s->sub[1].word;
        d->bytes[5] = s->sub[1].byte14;
        d->bytes[6] = (u8)s->sub[2].word;
        d->bytes[7] = s->sub[2].byte14;
        d->bytes[8] = (u8)s->sub[3].word;
        d->bytes[9] = s->sub[3].byte14;
        d->bytes[10] = (u8)s->sub[4].word;
        d->bytes[11] = s->sub[4].byte14;
        s++;
        d++;
    }
}
#pragma pop

// Invert func_8025EC0C: rebuild each 0xC4 slot from its compact 0xD form.
// Retail signature is (src=compact, dst=big) - r3 holds the compact struct.
extern "C" void func_8025ECE4(CPcKizunaCompact* src, CPcKizunagramBig* dst) {
    dst->field_0x89C = src->field_0xA8;
    dst->field_0x884 = src->field_0x90;
    memcpy(dst->data888, src->data94, 0x14);
    memset(dst->data870, 0, 0x14);
    // Pointer-walk form mirrors retail: outer cursors advance by 0xD/0xC4,
    // the sub cursor by 0x20 and the compact byte cursor by 2 per sub-entry.
    CPcKizunaCompactSlot* cs = &src->slots[0];
    CPcKizunaSlot* s = &dst->slots[0];
    int i = 0;
    while (i < 0xb) {
        s->byteC0 = cs->bytes[0xC];
        memset(&s->data00, 0, 0x20);
        s->data00.word = cs->bytes[0];
        u8* b = cs->bytes;
        // Two lagging cursors 0x20 apart (retail keeps both): cur is the
        // store base (+0x20/+0x34 displacements), sub feeds memset.
        CPcKizunaSlotEntry* cur = &s->data00;
        CPcKizunaSlotEntry* sub = cur + 1;
        s->data00.byte14 = cs->bytes[1];
        int k = 0;
        while (k < 5) {
            memset(sub, 0, 0x20);
            cur[1].word = b[2];
            cur[1].byte14 = b[3];
            k++;
            cur++;
            sub++;
            b += 2;
        }
        i++;
        s++;
        cs++;
    }
}

// Clear the 11 affinity slots (each 0xC4 bytes) and the trailing counters.
// Declaration order j/slot/sub/i drives the retail register coloring
// (r30/r29/r28/r27) under MWCC's descending declaration-order allocator.
extern "C" CPcKizunagramBig* func_8025EDC8(CPcKizunagramBig* self) {
    int j;
    CPcKizunaSlot* slot = &self->slots[0];
    CPcKizunaSlotEntry* sub;
    int i = 0;
    while (i < 0xb) {
        slot->byteC0 = 0;
        memset(slot, 0, 0x20);
        sub = &slot->sub[0];
        j = 0;
        while (j < 5) {
            memset(sub, 0, 0x20);
            j++;
            sub++;
        }
        i++;
        slot++;
    }
    memset(self->data870, 0, 0x14);
    memset(self->data888, 0, 0x14);
    self->field_0x884 = 0;
    self->field_0x89C = 0;
    self->field_0x86C = 0;
    return self;
}

void func_8025EE7C(CPcKizunagramBig* self, int r4) {
    if (self->field_0x89C != r4) {
        self->field_0x89C = r4;
        func_8025EE94(self);
    }
}

extern "C" void func_8025EE94(CPcKizunagramBig* self) {
    // Outer pass: for each of the first 5 slots, unlink its data00 entry from
    // the doubly-linked list, re-zero it (keeping only the low bit of byte14)
    // and clear the id bit from the persistence bitmap when the id isn't
    // duplicated elsewhere in the chart.
    for (int i = 0; i < 5; i++) {
        CPcKizunaSlot* slot = &self->slots[i];
        CPcKizunaSlotEntry* entry = &slot->data00;
        CPcKizunaSlotEntry* nxt = entry->pField18;
        u16 id = entry->field04;
        CPcKizunaSlotEntry* prv = entry->pField1C;
        if (nxt != 0) nxt->pField1C = prv;
        if (prv != 0) prv->pField18 = nxt;
        u8 saved = entry->byte14 & 1;
        memset(entry, 0, 0x20);
        entry->byte14 = saved;

        int found;
        if ((u32)id >= 0x9e) {
            found = 0;
        } else {
            // Search all 11 slots' 6 entries for a duplicate id. Constant trip
            // count drives the retail mtctr/bdnz loop (count hoisted before the
            // outer loop); found=0 after the loop emits the fallthrough li.
            CPcKizunaSlot* sp = &self->slots[0];
            for (int j = 0; j < 11; j++) {
                if (sp->data00.field04 == id) { found = 1; goto searchDone; }
                if (sp->sub[0].field04 == id) { found = 1; goto searchDone; }
                if (sp->sub[1].field04 == id) { found = 1; goto searchDone; }
                if (sp->sub[2].field04 == id) { found = 1; goto searchDone; }
                if (sp->sub[3].field04 == id) { found = 1; goto searchDone; }
                if (sp->sub[4].field04 == id) { found = 1; goto searchDone; }
                sp++;
            }
            found = 0;
        }
    searchDone:
        if (!found) {
            // Clear bit (id & 0x1f) of the 32-bit word at data870 + ((id>>3)&~7).
            u32 bit = 1u << (id & 0x1f);
            u32* p = (u32*)(self->data870 + ((id >> 3) & ~7));
            *p &= ~bit;
        }
    }

    // --- Second half: populate the current slot indicated by field_0x89C.
    int f89C = self->field_0x89C;
    int f86C = self->field_0x86C;
    u32 tbl = lbl_eu_8066415C;
    int total = f89C + (f86C - 1) * 5 + 1;
    CPcKizunaSlot* cur = &self->slots[f89C];

    // Find the first free sub-slot (word == 0) in the current slot.
    int idx;
    if (cur->sub[0].word == 0) {
        idx = 0;
    } else if (cur->sub[1].word == 0) {
        idx = 1;
    } else if (cur->sub[2].word == 0) {
        idx = 2;
    } else if (cur->sub[3].word == 0) {
        idx = 3;
    } else if (cur->sub[4].word == 0) {
        idx = 4;
    } else {
        idx = -1;
    }

    // Build the per-column-name format string from two 4-byte constants.
    char fmt[8];
    *(u32*)&fmt[0] = lbl_eu_806688A8;
    *(u32*)&fmt[4] = lbl_eu_806688AC;
    if (idx == -1) {
        fmt[3] = 0x36;
    } else {
        fmt[3] = (char)(idx + 0x31);
    }

    // Look up both column values first (retail keeps both on the stack across
    // the second call), then store them into the entry.
    const char* cols = lbl_eu_8050DB18;
    u32 v1 = getBdatStringColumnValue((void*)tbl, cols + 0x6, total);
    u32 v2 = getBdatStringColumnValue((void*)tbl, fmt, total);
    cur->data00.field04 = (u8)v1;
    cur->data00.field08 = (u8)v2;
    cur->data00.byte14 |= 2;

    // Set the new id's bit in the persistence bitmap.
    u16 newid = cur->data00.field04;
    u32* p2 = (u32*)(self->data870 + ((newid >> 3) & ~7));
    *p2 |= (1u << (newid & 0x1f));
    func_8025F114(self, &cur->data00);
}

// Persist an affinity entry (func_8025F114). If the entry's id is not yet in
// the persistence bitmap, set its bit and stop. Otherwise, when the entry is
// flagged for list insertion (byte14 bit 1), link it into the doubly-linked
// list that shares its id, inserting before the list head.
extern "C" void func_8025F114(CPcKizunagramBig* self, CPcKizunaSlotEntry* entry) {
    u32 id = entry->field04;
    u32* pWord = (u32*)(self->data870 + ((id >> 3) & ~7));
    u32 bit = 1u << (id & 0x1f);
    u32 word = *pWord;
    if ((word & bit) != 0) {
        if ((entry->byte14 & 2) == 0) return;

        // Search the whole chart for another entry with the same id, then
        // insert this entry before the head of that entry's prev chain.
        // Retail unrolls the head-walk 10 levels, then hands the remainder
        // to func_8025F290.
        for (int e = 0; e < 66; e++) {
            CPcKizunaSlotEntry* pos = &self->slots[e / 6].data00 + (e % 6);
            if (id != pos->field04) continue;
            if (pos == entry) continue;
            // Walk the prev chain toward the list head: 10 explicit gates
            // (retail unrolls this many levels), then hand the remainder to
            // the shared walker func_8025F290.
            CPcKizunaSlotEntry* head;
            CPcKizunaSlotEntry* cur = pos;
            CPcKizunaSlotEntry* nxt = cur->pField1C;
            if (nxt != 0) {
                cur = nxt;
                nxt = cur->pField1C;
                if (nxt != 0) {
                    cur = nxt;
                    nxt = cur->pField1C;
                    if (nxt != 0) {
                        cur = nxt;
                        nxt = cur->pField1C;
                        if (nxt != 0) {
                            cur = nxt;
                            nxt = cur->pField1C;
                            if (nxt != 0) {
                                cur = nxt;
                                nxt = cur->pField1C;
                                if (nxt != 0) {
                                    cur = nxt;
                                    nxt = cur->pField1C;
                                    if (nxt != 0) {
                                        cur = nxt;
                                        nxt = cur->pField1C;
                                        if (nxt != 0) {
                                            cur = nxt;
                                            nxt = cur->pField1C;
                                            if (nxt != 0) {
                                                cur = nxt;
                                                nxt = cur->pField1C;
                                                if (nxt != 0) {
                                                    head = func_8025F290(cur);
                                                } else {
                                                    head = cur;
                                                }
                                            } else {
                                                head = cur;
                                            }
                                        } else {
                                            head = cur;
                                        }
                                    } else {
                                        head = cur;
                                    }
                                } else {
                                    head = cur;
                                }
                            } else {
                                head = cur;
                            }
                        } else {
                            head = cur;
                        }
                    } else {
                        head = cur;
                    }
                } else {
                    head = cur;
                }
            } else {
                head = cur;
            }
            entry->pField1C = head;
            head->pField18 = entry;
            break;
        }
    } else {
        *pWord |= bit;
    }
}

// Walk the prev-pointer (0x1C) chain toward the list head, returning the node
// whose prev is null. Retail unrolls 5 levels then tail-calls itself. The
// nested-if shape with the n3-return kept flat reproduces retail's exact
// register rotation (n1=r4, n2=r5, n3=r3, n4=r4, n5=r3) and the bottom
// ret-blocks (beq-down for n1/n2/n4, beqlr for p/n3); the fully-flat or
// fully-nested forms give different layouts (probe-verified).
extern "C" __declspec(noinline) CPcKizunaSlotEntry* func_8025F290(CPcKizunaSlotEntry* p) {
    CPcKizunaSlotEntry* n1 = p->pField1C;
    if (n1 == 0) return p;
    CPcKizunaSlotEntry* n2 = n1->pField1C;
    if (n2 != 0) {
        CPcKizunaSlotEntry* n3 = n2->pField1C;
        if (n3 != 0) {
            CPcKizunaSlotEntry* n4 = n3->pField1C;
            if (n4 == 0) return n3;
            CPcKizunaSlotEntry* n5 = n4->pField1C;
            if (n5 != 0) {
                return func_8025F290(n5);
            }
            return n4;
        }
        return n2;
    }
    return n1;
}

// (Re)load an affinity-chart entry at (slot a, sub b) from the BDAT table:
// unlink and clear the entry (keeping byte14 bit 0), drop its persistence
// bitmap bit when the id no longer appears anywhere in the chart, then
// repopulate the six runtime columns and re-persist the entry.
extern "C" void func_8025F2E8(CPcKizunaChart* self, int a, int b, int value) {
    CPcKizunaSlotEntry* entry = &self->searchSlots[a].data00 + b;
    CPcKizunaSlotEntry* nxt = entry->pField18;
    u16 id = entry->field04;
    CPcKizunaSlotEntry* prv = entry->pField1C;
    if (nxt != 0) nxt->pField1C = prv;
    if (prv != 0) prv->pField18 = nxt;

    // Retail rematerializes the position from the raw indices after the
    // unlink consumes its first base register.
    CPcKizunaSlotEntry* pos = (CPcKizunaSlotEntry*)((u8*)&self->searchSlots[a] + b * 0x20);
    u8 saved = pos->byte14 & 1;
    memset(pos, 0, 0x20);

    int found;
    if ((u32)id >= 0x9e) {
        found = 0;
    } else {
        // Single-cursor walk over all 11 slots (stride 0xC4); the sub-entry
        // checks share a lagging cursor 0x40 past the slot base.
        const CPcKizunaSlot* sp = &self->searchSlots[0];
        int n = 0xb;
        do {
            const CPcKizunaSlotEntry* e = &sp->sub[1];
            if (sp->data00.field04 == id) { found = 1; goto searchDone; }
            if (sp->sub[0].field04 == id) { found = 1; goto searchDone; }
            if (e->field04 == id) { found = 1; goto searchDone; }
            if (e[1].field04 == id) { found = 1; goto searchDone; }
            if (e[2].field04 == id) { found = 1; goto searchDone; }
            if (e[3].field04 == id) { found = 1; goto searchDone; }
            sp++;
        } while (--n != 0);
        found = 0;
    }
searchDone:
    if (!found) {
        u32* pWord = (u32*)(self->data870 + ((id >> 3) & ~7));
        *pWord &= ~(1u << (id & 0x1f));
    }

    u32 tbl = lbl_eu_80664158;
    char* const cols = lbl_eu_8050DB18;
    entry->word = value;
    u32 v1 = getBdatStringColumnValue((void*)tbl, cols + 0x6, value);
    entry->field04 = (u8)v1;
    u32 v2 = getBdatStringColumnValue((void*)tbl, cols + 0xc, value);
    entry->field08 = (u8)v2;
    u32 v3 = getBdatStringColumnValue((void*)tbl, cols + 0x11, value);
    entry->field0C = (u8)v3;
    u32 v4 = getBdatStringColumnValue((void*)tbl, cols + 0x16, value);
    entry->field10 = (float)(u8)v4;
    u32 v5 = getBdatStringColumnValue((void*)tbl, cols + 0x1b, value);
    pos->byte14 |= (u8)v5;
    func_8025F114((CPcKizunagramBig*)self, entry);
}

// func_8025F528: same (re)load as func_8025F2E8 on the total chart.
extern "C" void func_8025F528(CPcKizunaChart* self, int a, int b, int value) {
    CPcKizunaSlotEntry* entry = &self->searchSlots[a].data00 + b;
    CPcKizunaSlotEntry* nxt = entry->pField18;
    u16 id = entry->field04;
    CPcKizunaSlotEntry* prv = entry->pField1C;
    if (nxt != 0) nxt->pField1C = prv;
    if (prv != 0) prv->pField18 = nxt;

    // Retail rematerializes the position from the raw indices after the
    // unlink consumes its first base register.
    CPcKizunaSlotEntry* pos = (CPcKizunaSlotEntry*)((u8*)&self->searchSlots[a] + b * 0x20);
    u8 saved = pos->byte14 & 1;
    memset(pos, 0, 0x20);

    int found;
    if ((u32)id >= 0x9e) {
        found = 0;
    } else {
        // Single-cursor walk over all 11 slots (stride 0xC4); the sub-entry
        // checks share a lagging cursor 0x40 past the slot base.
        const CPcKizunaSlot* sp = &self->searchSlots[0];
        int n = 0xb;
        do {
            const CPcKizunaSlotEntry* e = &sp->sub[1];
            if (sp->data00.field04 == id) { found = 1; goto searchDone; }
            if (sp->sub[0].field04 == id) { found = 1; goto searchDone; }
            if (e->field04 == id) { found = 1; goto searchDone; }
            if (e[1].field04 == id) { found = 1; goto searchDone; }
            if (e[2].field04 == id) { found = 1; goto searchDone; }
            if (e[3].field04 == id) { found = 1; goto searchDone; }
            sp++;
        } while (--n != 0);
        found = 0;
    }
searchDone:
    if (!found) {
        u32* pWord = (u32*)(self->data870 + ((id >> 3) & ~7));
        *pWord &= ~(1u << (id & 0x1f));
    }

    u32 tbl = lbl_eu_80664158;
    entry->word = value;
    char* const cols = lbl_eu_8050DB18;
    u32 v1 = getBdatStringColumnValue((void*)tbl, cols + 0x6, value);
    entry->field04 = (u8)v1;
    u32 v2 = getBdatStringColumnValue((void*)tbl, cols + 0xc, value);
    entry->field08 = (u8)v2;
    u32 v3 = getBdatStringColumnValue((void*)tbl, cols + 0x11, value);
    entry->field0C = (u8)v3;
    u32 v4 = getBdatStringColumnValue((void*)tbl, cols + 0x16, value);
    entry->field10 = (float)(u8)v4;
    u32 v5 = getBdatStringColumnValue((void*)tbl, cols + 0x1b, value);
    pos->byte14 |= (u8)v5;
    func_8025F114((CPcKizunagramBig*)self, entry);
}

// func_8025F768: same (re)load as func_8025F2E8, but on the per-character
// working copy, which sits 0x3D4 bytes past the matching total-chart position
// (union view). Retail holds the slot/sub offsets as scalars and re-derives
// the position pointer at each access, so every statement recomputes it.
#define KIZ_WORK_POS(chart, o, s) ((CPcKizunaWorkEntryPos*)((u8*)(chart) + (o) + (s)))

extern "C" void func_8025F768(CPcKizunaChart* self, int a, int b, int value) {
    u32 off = a * 0xC4;
    u32 sub = b << 5;

    // Unlink the working-copy entry from its id's doubly-linked list.
    CPcKizunaWorkEntryPos* wp = (CPcKizunaWorkEntryPos*)((u8*)self + off + sub);
    CPcKizunaSlotEntry* nxt = wp->entry.pField18;
    u16 id = wp->entry.field04;
    CPcKizunaSlotEntry* prv = wp->entry.pField1C;
    if (nxt != 0) nxt->pField1C = prv;
    if (prv != 0) prv->pField18 = nxt;

    // Zero the entry, keeping only bit 0 of byte14.
    CPcKizunaWorkEntryPos* wq = (CPcKizunaWorkEntryPos*)((u8*)self + sub + off);
    u8 saved = wq->entry.byte14 & 1;
    memset(&wq->entry, 0, 0x20);
    wq->entry.byte14 = saved;

    // Drop the persistence bitmap bit when the id no longer appears anywhere.
    int found;
    if ((u32)id >= 0x9e) {
        found = 0;
    } else {
        // Constant trip count drives the retail mtctr/bdnz loop; the sub-entry
        // checks share a lagging cursor 0x40 past the slot base.
        const CPcKizunaSlot* sp = &self->searchSlots[0];
        int n = 0xb;
        do {
            const CPcKizunaSlotEntry* e = &sp->sub[1];
            if (id == sp->data00.field04) { found = 1; goto searchDone; }
            if (id == sp->sub[0].field04) { found = 1; goto searchDone; }
            if (id == e->field04) { found = 1; goto searchDone; }
            if (id == e[1].field04) { found = 1; goto searchDone; }
            if (id == e[2].field04) { found = 1; goto searchDone; }
            if (id == e[3].field04) { found = 1; goto searchDone; }
            sp++;
        } while (--n != 0);
        found = 0;
    }
searchDone:
    if (!found) {
        u32* pWord = (u32*)(self->data870 + ((id >> 3) & ~7));
        *pWord &= ~(1u << (id & 0x1f));
    }

    // Repopulate the runtime columns from the BDAT table.
    u32 tbl = lbl_eu_80664158;
    CPcKizunaSlotEntry* entry =
        (CPcKizunaSlotEntry*)((u8*)self + off + sub + 0x3D4);
    entry->word = value;
    char* const cols = lbl_eu_8050DB18;
    u32 v1 = getBdatStringColumnValue((void*)tbl, cols + 0x6, value);
    wp->entry.field04 = (u8)v1;
    u32 v2 = getBdatStringColumnValue((void*)tbl, cols + 0xc, value);
    ((CPcKizunaSlotEntry*)((u8*)self + sub + off + 0x3D4))->field08 = (u8)v2;
    u32 v3 = getBdatStringColumnValue((void*)tbl, cols + 0x11, value);
    ((CPcKizunaSlotEntry*)((u8*)self + sub + off + 0x3D4))->field0C = (u8)v3;
    u32 v4 = getBdatStringColumnValue((void*)tbl, cols + 0x16, value);
    ((CPcKizunaSlotEntry*)((u8*)self + sub + off + 0x3D4))->field10 = (float)(u8)v4;
    u32 v5 = getBdatStringColumnValue((void*)tbl, cols + 0x1b, value);
    wq->entry.byte14 |= (u8)v5;
    func_8025F114((CPcKizunagramBig*)self, entry);
}

#pragma push
// Retail saves r28-r31 with individual stw's (speed-style frame), unlike the
// -O4,s stmw/helpers the unit default produces for this function.
#pragma optimize_for_size off
#pragma use_lmw_stmw off
extern "C" void func_8025F9AC(CPcKizunaChart* self, int a, int b) {
    // Retail keeps the total-chart position (self + a*0xC4 + b*0x20) as the
    // base register and reads the working-copy entry at +0x3D4 displacements.
    u32 off = a * 0xC4;
    u32 sub = b << 5;
    CPcKizunaSlotEntry* nxt;
    CPcKizunaSlotEntry* prv;
    u8 saved;
    u16 id;
    // The unlink block uses the position expression inline (a caller-saved
    // temp); only the clear block keeps the position in a nonvolatile.
    nxt = ((CPcKizunaWorkEntryPos*)((u8*)self + off + sub))->entry.pField18;
    id = ((CPcKizunaWorkEntryPos*)((u8*)self + off + sub))->entry.field04;
    prv = ((CPcKizunaWorkEntryPos*)((u8*)self + off + sub))->entry.pField1C;
    if (nxt != 0) nxt->pField1C = prv;
    if (prv != 0) prv->pField18 = nxt;

    // The retail re-derives the position base after the unlink, so recompute
    // it here instead of reusing the first pointer.
    // Operand order flipped vs the unlink expression so GVN keeps two
    // computations, matching retail's rematerialized base register.
    CPcKizunaWorkEntryPos* wq = (CPcKizunaWorkEntryPos*)((u8*)self + sub + off);
    saved = wq->entry.byte14 & 1;
    memset(&wq->entry, 0, 0x20);
    wq->entry.byte14 = saved;

    int found;
    if ((u32)id >= 0x9e) {
        found = 0;
    } else {
        CPcKizunaSlot* sp = &self->searchSlots[0];
        for (int j = 0; j < 11; j++) {
            const CPcKizunaSlotEntry* e = &sp->sub[1];
            if (sp->data00.field04 == id) { found = 1; goto searchDone; }
            if (sp->sub[0].field04 == id) { found = 1; goto searchDone; }
            if (e->field04 == id) { found = 1; goto searchDone; }
            if (e[1].field04 == id) { found = 1; goto searchDone; }
            if (e[2].field04 == id) { found = 1; goto searchDone; }
            if (e[3].field04 == id) { found = 1; goto searchDone; }
            sp++;
        }
        found = 0;
    }
searchDone:
    if (!found) {
        u32* p = (u32*)(self->data870 + ((id >> 3) & ~7));
        *p &= ~(1u << (id & 0x1f));
    }
}
#pragma pop
