// TU for kyoshin/CPcKizunagram - PC affinity chart window.
// func_8025DA40 / func_8025DA48: FULL_MATCH (byte-identical).

#include "kyoshin/CPcKizunagram.hpp"

#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"

// Forward declarations of intra-TU callees (free unmangled functions).
void func_8025E56C(CPcKizunagram* self);
void func_8025E5A8(CPcKizunagram* self);
void func_8025E5E4(CPcKizunagram* self, u32 value);
void func_8025EE94(CPcKizunagramBig* self);
void func_8025E0D8(CPcKizunagram* self);

// Layout animation helpers from code_80135FDC (retail unmangled func_80137510).
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

// String base for the affinity-chart layout paths (targets add byte offsets).
extern char lbl_eu_8050D868[];

// Column-name format base (func_8025EE94 uses +0x06).
extern char lbl_eu_8050DB18[];

void func_8025F114();

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
CPcKizunaCur::CPcKizunaCur(nw4r::lyt::ArcResourceAccessor* accessor) {
    mVtable = lbl_eu_805376AC;
    mAccessor = accessor;
    mpLayout = 0;
    mpAnim0 = 0;
    mpAnim1 = 0;
    mField14 = 0;
    mField15 = 1;
    mField16 = 0;
}

CPcKizunaCur::~CPcKizunaCur() {}

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
    new (reinterpret_cast<CPcKizunaCur*>(mKizunaCur)) CPcKizunaCur(0);
    mByte44 = 0;
    mFloat48 = lbl_eu_80668880;
}

extern "C" void func_8025D6E0(CPcKizunaTreeRoot* self, const CPcKizunaVec3* src) {
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

void func_8025DA78(){}

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
    if (self->mByte44 == 0) {
        func_8025E56C(self);
    } else if (self->mByte44 == 1) {
        func_8025E5A8(self);
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
    int count = func_8003B1EC() & 0xff;
    func_8009CF8C(0x20);

    for (int n = 1; n <= count; n++) {
        char* paneName = func_8013639C(table, lbl_eu_8050D868 + 0x90, (u8)n);
        nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(paneName, true);
        if (func_8025E960(self, table, (u8)n) != 0) {
            // --- selected row ---
            if (pane != 0) func_80124270(pane, 1);
            if (func_8025E9E4(self, table, (u8)n) != 0) {
                func_80137C1C(pane, 0x7777ffff);
            }
            for (int k = 1; k <= 6; k++) {
                char buf[0x20];
                sprintf(buf, lbl_eu_8050D868 + 0x96, (u8)k);
                u32 e8 = func_801361E8(table, buf, (u8)n);
                int rnd = (e8 & 0xff) + 0x29;
                func_8009CF8C(rnd);
                u8 lb8 = 0, lb9 = 0;
                func_8013AB0C(&lb9, &lb8, rnd);
                int r21 = 0;
                if (func_8025E904(self, table, lb9) != 0 && func_8025E904(self, table, lb8) != 0) {
                    r21 = 1;
                }
                int id = (e8 & 0xff) + 1;
                char* str = func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xaf, id);
                nw4r::lyt::Pane* p = self->mLayout->GetRootPane()->FindPaneByName(str, true);
                if (p != 0) func_80124270(p, r21);
                if (r21 != 0) {
                    void* tex = self->mArcRes->GetResource(0x74696d67, lbl_eu_8050D868 + 0xb5, 0);
                    if (tex != 0) func_80137F88(p, tex);
                }
                char* str2 = func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xca, id);
                nw4r::lyt::Pane* p2 = self->mLayout->GetRootPane()->FindPaneByName(str2, true);
                if (p2 != 0) func_80124270(p2, 0);
                char* str3 = func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xd4, id);
                nw4r::lyt::Pane* p3 = self->mLayout->GetRootPane()->FindPaneByName(str3, true);
                if (p3 != 0) func_80124270(p3, 0);
            }
        } else {
            // --- deselected row ---
            if (pane != 0) func_80124270(pane, 0);
            if ((u8)n == 5) {
                u16 r25 = (u16)func_8013606C(lbl_eu_8050D868 + 0x7f, lbl_eu_8050D868 + 0xdd, 5);
                if (func_8009CF8C(0x20) >= r25) {
                    for (int k = 1; k <= 6; k++) {
                        char buf[0x20];
                        sprintf(buf, lbl_eu_8050D868 + 0x96, (u8)k);
                        u32 e8 = func_801361E8(table, buf, (u8)n);
                        int id = (e8 & 0xff) + 1;
                        char* str = func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xaf, id);
                        nw4r::lyt::Pane* p = self->mLayout->GetRootPane()->FindPaneByName(str, true);
                        if (p != 0) func_80124270(p, 0);
                        char* str2 = func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xca, id);
                        nw4r::lyt::Pane* p2 = self->mLayout->GetRootPane()->FindPaneByName(str2, true);
                        if (p2 != 0) func_80124270(p2, 0);
                        char* str3 = func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xd4, id);
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
    int count = func_8003B1EC() & 0xff;
    for (int i = 0; i < count; i++) {
        func_8013639C(table, lbl_eu_8050D868 + 0x90, (u8)i + 1);
    }

    // Second pass: refresh the 6 affinity rows.
    for (int k = 1; k <= 6; k++) {
        char buf[0x20];
        sprintf(buf, lbl_eu_8050D868 + 0x96, (u8)k);
        u32 e8 = func_801361E8(table, buf, (s8)self->mField28 + 1);
        u16 val = (u16)func_8009CF8C((e8 & 0xff) + 0x29);
        int id = (e8 & 0xff) + 1;

        char* str1 = func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xaf, id);
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
        void* tex1 = self->mArcRes->GetResource(0x74696d67, s1, 0);
        if (tex1 != 0) func_80137F88(pane, tex1);

        char* str2 = func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xca, id);
        const char* s2;
        if (val >= 0xbb8) {
            s2 = (val >= 0x2711) ? 0 : (val >= 0x1388) ? lbl_eu_8050D868 + 0x1ad : lbl_eu_8050D868 + 0x195;
        } else if (val >= 0x7d0) {
            s2 = lbl_eu_8050D868 + 0x17d;
        } else {
            s2 = (val >= 0x3e8) ? lbl_eu_8050D868 + 0x165 : lbl_eu_8050D868 + 0x14d;
        }
        void* tex2 = self->mArcRes->GetResource(0x74696d67, s2, 0);
        if (tex2 != 0) {
            nw4r::lyt::Pane* pane2 = self->mLayout->GetRootPane()->FindPaneByName(str2, true);
            func_80137F88(pane2, tex2);
            if (pane2 != 0) func_80124270(pane2, 1);
        }

        char* str3 = func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xd4, id);
        func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->mLayout, str3, (u8)val);
        nw4r::lyt::Pane* pane3 = self->mLayout->GetRootPane()->FindPaneByName(str3, true);
        if (pane3 != 0) func_80124270(pane3, 1);
    }
}


// Initialise the per-character cursor widget: load its layout and two animation
// transforms, enable the idle anim, pin down two named panes, then reset the
// root pane scale.
extern "C" void func_8025D4E4(CPcKizunaCur* self) {
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
    S8Bytes order;
    order.w[0] = lbl_eu_80668888.w[0];
    order.w[1] = lbl_eu_80668888.w[1];
    func_8003AA34();
    const void* table = getFP__FPCc(lbl_eu_8050D868 + 0x7f);
    int count = func_8003B1EC();
    // Locate the current character row inside the 8-entry order list.
    u32 idx = 0;
    for (u32 k = 0; k < 8; k++) {
        if (order.b[k] == (s8)self->mField28) {
            idx = k;
            break;
        }
    }
    // Step the cursor and commit the first BDAT-valid target row found.
    int tries = 0;
    int idx2 = (int)idx;
    do {
        if (arg != 0) {
            idx2 = idx2 - 1;
            if (idx2 < 0) idx2 = 7;
        } else {
            idx2 = idx2 + 1;
            if (idx2 > 7) idx2 = 0;
        }
        if (func_8025E960(self, table, (u8)((u8)order.b[idx2] + 1)) != 0) {
            self->mField28 = order.b[idx2];
            break;
        }
        tries++;
    } while (tries <= count);
}

// Target a text pane by name, then move the cursor so it sits over the row
// matching the current character index.
extern "C" void __declspec(noinline) func_8025E4A4(CPcKizunagram* self) {
    char* path = func_80136190(lbl_eu_8050D868 + 0x7f, lbl_eu_8050D868 + 0x90, (s8)self->mField28 + 1);
    nw4r::lyt::Pane* pane1 = self->mLayout->GetRootPane()->FindPaneByName(path, true);
    nw4r::lyt::Pane* pane2 = self->mLayout->GetRootPane()->FindPaneByName(lbl_eu_8050D868 + 0x1c5, true);
    CPcKizunaVec3 pos;
    func_80137924(&pos, (void*)pane1, (void*)pane2, (void*)self->mLayout->GetRootPane());
    CPcKizunaVec3 tmp;
    tmp.x = pos.x;
    tmp.y = pos.y;
    tmp.z = pos.z;
    func_8025D6E0((CPcKizunaTreeRoot*)self->mKizunaCur, &tmp);
}

void func_8025E56C(CPcKizunagram* self) {
    self->mFloat48 += lbl_eu_8066887C;
    if (self->mFloat48 >= lbl_eu_80668890) {
        self->mByte44 = 1;
        self->mFloat48 = lbl_eu_80668880;
        func_8025E5E4(self, 1);
    }
}

void func_8025E5A8(CPcKizunagram* self) {
    self->mFloat48 += lbl_eu_8066887C;
    if (self->mFloat48 >= lbl_eu_80668894) {
        self->mByte44 = 0;
        self->mFloat48 = lbl_eu_80668880;
        func_8025E5E4(self, 0);
    }
}

void func_8025E5E4(CPcKizunagram* self, u32 value) {
    const void* table = getFP__FPCc(lbl_eu_8050D868 + 0x7f);
    u8 count = (u8)func_8003B1EC();
    for (u8 i = 0; i < count; i++) {
        func_8013639C(table, lbl_eu_8050D868 + 0x90, (u8)i + 1);
    }

    for (u32 k = 1; k <= 6; k++) {
        char buf1[0x20];
        sprintf(buf1, lbl_eu_8050D868 + 0x96, (u8)k);
        u32 e8 = func_801361E8(table, buf1, (s8)self->mField28 + 1);
        u16 val = (u16)func_8009CF8C((e8 & 0xff) + 0x29);
        int id = (e8 & 0xff) + 1;

        char* str1 = func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xaf, id);
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
        void* tex1 = self->mArcRes->GetResource(0x74696d67, s1, 0);
        if (tex1 != 0) func_80137F88(pane, tex1);

        // Second panel: build its resource name from a value-dependent format.
        // Each range chooses a distinct format string; the val>=0x2711 range
        // skips the sprintf entirely (falls straight to the resource fetch).
        char* str2 = func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xca, id);
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
        void* tex2 = self->mArcRes->GetResource(0x74696d67, buf2, 0);
        if (tex2 != 0) {
            nw4r::lyt::Pane* pane2 = self->mLayout->GetRootPane()->FindPaneByName(str2, true);
            func_80137F88(pane2, tex2);
            if (pane2 != 0) func_80124270(pane2, 1);
        }

        char* str3 = func_80136190(lbl_eu_8050D868 + 0xa0, lbl_eu_8050D868 + 0xd4, id);
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
extern "C" int func_8025E960(CPcKizunagram* self, const void* table, int id) {
    u16 v1 = (u16)func_80136254(table, lbl_eu_8050D868 + 0xdd, (const void*)id);
    u16 v2 = (u16)func_80136254(table, lbl_eu_8050D868 + 0x256, (const void*)id);
    u32 check = func_8009CF8C(0x20) & 0xFFFF;
    int result = 0;
    if (v1 <= check && check <= v2) result = 1;
    return result;
}

extern "C" int func_8025E9E4(CPcKizunagram* self, const void* table, int id) {
    u16 v1 = (u16)func_80136254(table, lbl_eu_8050D868 + 0x25d, (const void*)id);
    u16 v2 = (u16)func_80136254(table, lbl_eu_8050D868 + 0x264, (const void*)id);
    u32 check = func_8009CF8C(0x20) & 0xFFFF;
    int result = 0;
    if (v1 <= check && check <= v2) result = 1;
    return result;
}

void CPcKizunagram::OnFileEvent() {}

// Compress an 11-slot CPcKizunagramBig into the compact CPcKizunaCompact:
// each 0xC4 slot collapses to 0xD bytes (the set state bytes only).
extern "C" void func_8025EC0C(CPcKizunaCompact* dst, const CPcKizunagramBig* src) {
    dst->field_0xA8 = src->field_0x89C;
    dst->field_0x90 = src->field_0x884;
    memcpy(dst->data94, src->data888, 0x14);
    for (int i = 0; i < 0xb; i++) {
        const CPcKizunaSlot* s = &src->slots[i];
        CPcKizunaCompactSlot* d = &dst->slots[i];
        d->bytes[0xC] = s->byteC0;
        d->bytes[0] = (u8)s->data00.word;
        d->bytes[1] = s->data00.byte14;
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
    }
}

// Invert func_8025EC0C: rebuild each 0xC4 slot from its compact 0xD form.
extern "C" void func_8025ECE4(CPcKizunagramBig* dst, const CPcKizunaCompact* src) {
    dst->field_0x89C = src->field_0xA8;
    dst->field_0x884 = src->field_0x90;
    memcpy(dst->data888, src->data94, 0x14);
    memset(dst->data870, 0, 0x14);
    for (int i = 0; i < 0xb; i++) {
        dst->slots[i].byteC0 = src->slots[i].bytes[0xC];
        memset(&dst->slots[i].data00, 0, 0x20);
        dst->slots[i].data00.word = src->slots[i].bytes[0];
        dst->slots[i].data00.byte14 = src->slots[i].bytes[1];
        for (int k = 0; k < 5; k++) {
            memset(&dst->slots[i].sub[k], 0, 0x20);
            dst->slots[i].sub[k].word = src->slots[i].bytes[2 + 2 * k];
            dst->slots[i].sub[k].byte14 = src->slots[i].bytes[3 + 2 * k];
        }
    }
}

// Clear the 11 affinity slots (each 0xC4 bytes) and the trailing counters.
extern "C" CPcKizunagramBig* func_8025EDC8(CPcKizunagramBig* self) {
    int i = 0;
    CPcKizunaSlot* slot = &self->slots[0];
    while (i < 0xb) {
        slot->byteC0 = 0;
        memset(slot, 0, 0x20);
        int j = 0;
        CPcKizunaSlotEntry* sub = &slot->sub[0];
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

void func_8025EE94(CPcKizunagramBig* self) {
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
        if (id < 0x9e) {
            // Search all 11 slots' 6 entries for a duplicate id.
            found = 0;
            CPcKizunaSlot* sp = &self->slots[0];
            int n = 11;
            do {
                if (sp->data00.field04 == id) { found = 1; break; }
                if (sp->sub[0].field04 == id) { found = 1; break; }
                if (sp->sub[1].field04 == id) { found = 1; break; }
                if (sp->sub[2].field04 == id) { found = 1; break; }
                if (sp->sub[3].field04 == id) { found = 1; break; }
                if (sp->sub[4].field04 == id) { found = 1; break; }
                sp++;
            } while (--n != 0);
        } else {
            found = 0;
        }
        if (!found) {
            // Clear bit (id & 0x1f) of the 32-bit word at data870 + ((id>>3)&~7).
            u32* p = (u32*)(self->data870 + ((id >> 3) & ~7));
            *p &= ~(1u << (id & 0x1f));
        }
    }

    // --- Second half: populate the current slot indicated by field_0x89C.
    u32 tbl = *(u32*)&lbl_eu_8066415C;
    int f86C = self->field_0x86C;
    u32 f89C = self->field_0x89C;
    CPcKizunaSlot* cur = &self->slots[f89C];
    int total = (int)f89C + (f86C - 1) * 5 + 1;

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
    *(u32*)&fmt[0] = *(u32*)&lbl_eu_806688A8;
    *(u32*)&fmt[4] = *(u32*)&lbl_eu_806688AC;
    fmt[3] = (idx == -1) ? 0x36 : (char)(idx + 0x31);

    // Look up and store the affinity value and the row id.
    u8 v1 = (u8)getBdatStringColumnValue((void*)tbl, lbl_eu_8050DB18 + 0x6, total);
    cur->data00.field04 = v1;
    u32 v2 = getBdatStringColumnValue((void*)tbl, fmt, total);
    cur->data00.field08 = (u8)v2;
    cur->data00.byte14 |= 2;

    // Set the new id's bit in the persistence bitmap.
    u16 newid = cur->data00.field04;
    u32* p2 = (u32*)(self->data870 + ((newid >> 3) & ~7));
    *p2 |= (1u << (newid & 0x1f));
    func_8025F114();
}

void func_8025F114(){}

void func_8025F290(){}

void func_8025F2E8(){}

void func_8025F528(){}

void func_8025F768(){}

void func_8025F9AC(){}
