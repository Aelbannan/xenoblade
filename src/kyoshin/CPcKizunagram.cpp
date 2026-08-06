// TU for kyoshin/CPcKizunagram — PC affinity chart window.
// func_8025DA40 / func_8025DA48: FULL_MATCH (byte-identical).

#include "kyoshin/CPcKizunagram.hpp"

#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"

// Forward declarations of intra-TU callees (free unmangled functions).
extern "C" void func_8025DCFC(CPcKizunagram* self);
extern "C" void func_8025E3A4(CPcKizunagram* self, u32 arg);
extern "C" void func_8025E4A4(CPcKizunagram* self);
void func_8025E56C(CPcKizunagram* self);
void func_8025E5A8(CPcKizunagram* self);
void func_8025E5E4(u32 arg);
void func_8025EE94(CPcKizunagramBig* self);

// Sound / effect helper (C-linkage retail symbol func_80138078__FUl).
extern "C" void func_80138078__FUl(u32 arg);

// Layout animation helpers from code_80135FDC (retail unmangled func_80137510).
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);
void func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

// String base for the affinity-chart layout paths (targets add byte offsets).
extern char lbl_eu_8050D868[];

// Named float constants in .sdata2 referenced via @sda21 by func_8025E56C/E5A8.
extern const float lbl_eu_8066887C;
extern const float lbl_eu_80668880;
extern const float lbl_eu_80668890;
extern const float lbl_eu_80668894;

u8 func_8025DA40(CPcKizunagram* pKizunagram) { return pKizunagram->mIsHidden; }










void func_8025D8C4(){}

// Draw the main layout and, if present, the embedded cursor layout.
void func_8025D954(CPcKizunagram* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->mStateByte1 == 0) return;
    if (self->mStateByte2 == 0) return;
    func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(self->mLayout, drawInfo, 0, 1);
    nw4r::lyt::Layout* cursorLayout = ((CPcKizunaCur*)self->mKizunaCur)->mpLayout;
    if (cursorLayout != 0) {
        func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(cursorLayout, drawInfo, 0, 1);
    }
}

void func_8025D9C4(){}


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

void func_8025D6E0(CPcKizunaTreeRoot* self, const CPcKizunaVec3* src) {
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

void func_8025DC08(){}

// Dispatch on the state byte at +0x44; each branch is a tail call.
void func_8025DC8C(CPcKizunagram* self) {
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
void func_8025DCB0(CPcKizunagram* self) {
    if (func_80137510(self->mAnimTransform, lbl_eu_8066887C) != 0) {
        self->mStateByte2 = 0;
        self->mIsOpen = 1;
    }
}

// These three are large retail functions (0x100/0xc8/0x3dc) not yet
// implemented here; noinline keeps callers emitting real bl instructions, and
// extern "C" keeps the unmangled retail symbol names.
extern "C" __declspec(noinline) void func_8025DCFC(CPcKizunagram* self){}

void func_8025E0D8(){}

extern "C" __declspec(noinline) void func_8025E3A4(CPcKizunagram* self, u32 arg){}

extern "C" __declspec(noinline) void func_8025E4A4(CPcKizunagram* self){}

void func_8025E56C(CPcKizunagram* self) {
    self->mFloat48 += lbl_eu_8066887C;
    if (self->mFloat48 >= lbl_eu_80668890) {
        self->mByte44 = 1;
        self->mFloat48 = lbl_eu_80668880;
        func_8025E5E4(1);
    }
}

void func_8025E5A8(CPcKizunagram* self) {
    self->mFloat48 += lbl_eu_8066887C;
    if (self->mFloat48 >= lbl_eu_80668894) {
        self->mByte44 = 0;
        self->mFloat48 = lbl_eu_80668880;
        func_8025E5E4(0);
    }
}

void func_8025E5E4(u32 arg){}

void func_8025E904(){}

void func_8025E960(){}

void func_8025E9E4(){}

void CPcKizunagram::OnFileEvent() {}

void func_8025EC0C(){}

void func_8025ECE4(){}

void func_8025EDC8(){}

void func_8025EE7C(CPcKizunagramBig* self, int r4) {
    if (self->field_0x89C != r4) {
        self->field_0x89C = r4;
        func_8025EE94(self);
    }
}

void func_8025EE94(CPcKizunagramBig* self){}

void func_8025F114(){}

void func_8025F290(){}

void func_8025F2E8(){}

void func_8025F528(){}

void func_8025F768(){}

void func_8025F9AC(){}
