// Auto-scaffolded catalog TU for kyoshin/CArtsInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CArtsInfo.hpp"
#include <nw4r/lyt.h>
#include <stdlib.h>

// Extern declarations for referenced symbols

// String table base (rodata, accessed via sda21 relocation)
extern char lbl_eu_8050B00C[];

// Vtable symbol for CArtsInfo
extern void* lbl_eu_80536A88;

// Jump table for state machine dispatch (func_8023587C)
extern void* jumptable_eu_805369A0[];

// Float constants in small data area
extern float lbl_eu_80668680; // 0.0f
extern const float lbl_eu_80668684; // 1.0f (const: retail hoists the pool load)

// Small data objects (.sdata, addressed via @sda21): declared as complete
// 4-byte objects so MWCC emits EMB_SDA21 relocs like retail (an incomplete
// char[] extern would force lis/addi absolute addressing).
extern u32 lbl_eu_80664748;
extern u32 lbl_eu_80664750;
extern u32 lbl_eu_80664758;
extern u32 lbl_eu_80664760;

// Small data object (.sbss, addressed via @sda21): arts bdat file pointer.
extern u32 lbl_eu_806640F4;

void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

// Forward declarations for state machine functions (defined later in this TU).
// func_802369C0/func_80236CF4 are extern "C": retail calls the C names (not
// __FP9CArtsInfo).
void func_80236508(CArtsInfo*);
extern "C" void func_802369C0(CArtsInfo*);
extern "C" void func_80236CF4(CArtsInfo*);

// Forward declarations for animation state handlers
void func_80235F6C(CArtsInfo*);
void func_80236020(CArtsInfo*);
void func_8023606C(CArtsInfo*);
void func_80236120(CArtsInfo*);
void func_8023616C(CArtsInfo*);
void func_80236220(CArtsInfo*);
void func_802362D4(CArtsInfo*);
void func_80236334(CArtsInfo*);
void func_80236408(CArtsInfo*);
void func_80236454(CArtsInfo*);

// Virtual method call helpers (offset 0x38 = Animate-like, offset 0x2C = BindAnim-like)
static inline void callVirt_38_0(nw4r::lyt::Layout* layout) {
    typedef void (*VirtFn)(nw4r::lyt::Layout*, u32);
    VirtFn fn = ((VirtFn*)*(u32*)layout)[14]; // 0x38 / 4 = 14
    fn(layout, 0);
}

static inline void callVirt_2C_50(nw4r::lyt::Layout* layout, nw4r::lyt::AnimTransform* arg1, u32 arg2) {
    typedef void (*VirtFn)(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform*, u32);
    VirtFn fn = ((VirtFn*)*(u32*)layout)[11]; // 0x2C / 4 = 11
    fn(layout, arg1, arg2);
}

// Vtable virtual method at offset 0x08 with flags=1 (destructor)
static inline void callVirtDelete_08(nw4r::lyt::Layout* obj) {
    typedef void (*VirtFn)(nw4r::lyt::Layout*, int);
    VirtFn fn = ((VirtFn*)*(u32*)obj)[2]; // 0x08 / 4 = 2
    fn(obj, 1);
}

// Vtable virtual method at offset 0x0C
static inline void callVirt_0C(void* obj) {
    typedef void (*VirtFn)(void*);
    VirtFn fn = ((VirtFn*)*(u32*)obj)[3]; // 0x0C / 4 = 3
    fn(obj);
}

// Vtable virtual method at offset 0x10 with pointer arg
static inline void callVirt_10(void* obj, void* arg) {
    typedef void (*VirtFn)(void*, void*);
    VirtFn fn = ((VirtFn*)*(u32*)obj)[4]; // 0x10 / 4 = 4
    fn(obj, arg);
}

// Vtable virtual method at offset 0x3C with two args
static inline u32 callVirt_3C(nw4r::lyt::Layout* layout, const char* arg1, u32 arg2) {
    typedef u32 (*VirtFn)(nw4r::lyt::Layout*, const char*, u32);
    VirtFn fn = ((VirtFn*)*(u32*)layout)[15]; // 0x3C / 4 = 15
    return fn(layout, arg1, arg2);
}

u8 CArtsInfo::getField48() { return field_0x48; }

u8 CArtsInfo::getField49() { return field_0x49; }

// __ct__CArtsInfo - constructor
// .text:0x0, size 0xAC
CArtsInfo::CArtsInfo() {
    // Set vtable pointer
    *(void**)this = &lbl_eu_80536A88;

    // Construct embedded UnkClass_8045F564 member
    __ct__17UnkClass_8045F564Fv(&mMemRegion);

    // Initialize all fields to zero/default
    field_0x14 = 0;
    field_0x18 = 0;
    field_0x1C = 0;
    mpLayout1 = nullptr;
    mpAnimTrans1 = nullptr;
    mpAnimTrans2 = nullptr;
    mpAnimTrans3 = nullptr;
    mpAnimTrans4 = nullptr;
    mpLayout2 = nullptr;
    mpAnimTrans5 = nullptr;
    mpAnimTrans6 = nullptr;
    field_0x40 = 0;
    field_0x44 = 0;
    field_0x48 = 0;
    field_0x49 = 1;
    field_0x4C = 0;
    field_0x50 = 0;
    field_0x54 = 0;
    field_0x55 = 0;
    field_0x56 = 0;
    field_0x58 = 0;
    field_0x5A = 0;

    // Construct embedded CCur18 cursor
    __ct__CCur18(mCursor, nullptr);
}

// __dt__9CArtsInfoFv - destructor
// .text:0xAC, size 0x60
// Retail dtor is a plain function: destroys the +0x5C CCur18 cursor and the
// +0x04 UnkClass_8045F564 member, then frees self when the delete flag is
// positive. Written in the free-function form (CTitle pattern) because the
// member-dtor shape would emit a vtable store for the virtual dtor that
// retail does not have. optimize_for_size merges the r30/r31 saves into the
// retail stmw r30 prologue.
#pragma optimize_for_size on
extern "C" void* __dt__9CArtsInfoFv(void* self, int flags) {
    if (self != 0) {
        __dt__6CCur18Fv((u8*)self + 0x5C, -1);
        __dt__17UnkClass_8045F564Fv((u8*)self + 0x04, -1);
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}
#pragma optimize_for_size off

// func_80235814 - file loading
// .text:0x10C, size 0x68
#pragma optimize_for_size on
void func_80235814(CArtsInfo* self) {
    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    self->field_0x14 = (int)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)handle, (const char*)lbl_eu_8050B00C, (void*)self, 0, 0);

    self->field_0x18 =
        (int)readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(
            (u32)func_800A9D90(), (const char*)(lbl_eu_8050B00C + 0x15),
            (void*)self, 0, 0);
}
#pragma optimize_for_size off

// func_8023587C - state machine dispatch
// .text:0x174, size 0xDC
void func_8023587C(CArtsInfo* self) {
    if (self->field_0x40 == 0) return;

    int state = self->field_0x44;
    if (state >= 0 && state <= 0xC) {
        // Dispatch based on state via jump table
        switch (state) {
        case 0: func_80235F6C(self); break;
        case 1: func_80236020(self); break;
        case 2: func_8023606C(self); break;
        case 3: func_80236120(self); break;
        case 4: func_8023616C(self); break;
        case 5: func_80236220(self); break;
        case 6: func_802362D4(self); break;
        case 7: func_80236334(self); break;
        case 8: func_80236408(self); break;
        case 9: func_80236454(self); break;
        // cases 0xA-0xC fall through to default
        default: break;
        }
    }

    // Post-dispatch: animate both layouts and update cursor
    callVirt_38_0(self->mpLayout1);
    callVirt_38_0(self->mpLayout2);
    func_801D202C(self->mCursor);
}

// func_80235958 - draw
// .text:0x250, size 0x74
// optimize_for_size gives the retail stmw r30 prologue for the 2 saved regs.
#pragma optimize_for_size on
void func_80235958(CArtsInfo* self, void* drawInfo) {
    if (self->field_0x40 == 0) return;

    func_80137038(self->mpLayout1, reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
    func_80137038(self->mpLayout2, reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);

    s8 cursorActive = self->field_0x5A;
    if (cursorActive >= 0) {
        func_801D20B0(self->mCursor, drawInfo);
    }
}
#pragma optimize_for_size off

// func_802359CC - cleanup
// .text:0x2C4, size 0xC4
void func_802359CC(CArtsInfo* self) {
    func_8003AA8C__5CBdatFUl(2);

    func_801390E0__FPP11CFileHandle(&self->field_0x14);
    func_801390E0__FPP11CFileHandle(&self->field_0x18);

    self->field_0x40 = 0;

    // Delete layout 1 if non-null
    if (self->mpLayout1 != nullptr) {
        callVirtDelete_08(self->mpLayout1);
        self->mpLayout1 = nullptr;
    }

    // Delete layout 2 if non-null
    if (self->mpLayout2 != nullptr) {
        callVirtDelete_08(self->mpLayout2);
        self->mpLayout2 = nullptr;
    }

    // Cleanup arc resource accessor
    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(&self->field_0x1C);

    // Call virtual cleanup on cursor
    callVirt_0C(self->mCursor);

    // Cleanup memory region
    func_8045F778__17UnkClass_8045F564Fv(&self->mMemRegion);
}

// func_80235A90 - get field_0x48
// .text:0x388, size 0x8
u8 func_80235A90(CArtsInfo* self) { return self->field_0x48; }

// func_80235A98 - get field_0x49
// .text:0x390, size 0x8
u8 func_80235A98(CArtsInfo* self) { return self->field_0x49; }

// func_80235AA0 - state check (state==0 -> 1)
// .text:0x398, size 0x20
void func_80235AA0(CArtsInfo* self) {
    if (self->field_0x44 != 0) return;
    self->field_0x44 = 1;
    self->field_0x49 = 0;
}

// func_80235AC0 - state check (state==3 -> 4)
// .text:0x3B8, size 0x20
void func_80235AC0(CArtsInfo* self) {
    if (self->field_0x44 != 3) return;
    self->field_0x44 = 4;
    self->field_0x49 = 0;
}

// func_80235AE0 - large state machine (state==3 -> 6, layout animation setup)
// .text:0x3D8, size 0x244
void func_80235AE0(CArtsInfo* self) {
    if (self->field_0x44 != 3) return;

    self->field_0x44 = 6;
    self->field_0x49 = 0;

    // Bind animations to layout 1
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans2, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans4, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans3, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans1, 1);

    // Set anim transform frame
    *(float*)((u8*)self->mpAnimTrans4 + 0x10) = lbl_eu_80668680;

    callVirt_38_0(self->mpLayout1);

    // Bind animations to layout 1 again
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans2, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans4, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans1, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans3, 1);

    // Set anim transform frame
    *(float*)((u8*)self->mpAnimTrans3 + 0x10) = lbl_eu_80668680;

    callVirt_38_0(self->mpLayout1);

    // Bind animations to layout 2
    callVirt_2C_50(self->mpLayout2, self->mpAnimTrans6, 0);
    callVirt_2C_50(self->mpLayout2, self->mpAnimTrans5, 1);

    // Set anim transform frame
    *(float*)((u8*)self->mpAnimTrans5 + 0x10) = lbl_eu_80668680;

    callVirt_38_0(self->mpLayout2);

    // Reset cursor state
    self->field_0x5A = 0;

    // Update layout elements with arts data
    u32 artsData = (u32)func_8009EC9C(self->field_0x54);
    char* str1 = (char*)func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3D, 0x18);

    char buf[64];
    sprintf(buf, lbl_eu_8050B00C + 0x42, *(u32*)((u8*)artsData + 0x17C), str1);

    func_80136A1C(self->mpLayout1, lbl_eu_8050B00C + 0x47, buf, 0);

    func_80138078__FUl(0x6D);
}

// func_80235D24 - state machine (state==9 -> 0xA)
// .text:0x61C, size 0xB4
void func_80235D24(CArtsInfo* self) {
    if (self->field_0x44 != 9) return;

    self->field_0x44 = 0xA;
    self->field_0x49 = 0;

    // Set cursor visibility
    func_801D216C(self->mCursor, 0);

    // Bind animations to layout 2
    callVirt_2C_50(self->mpLayout2, self->mpAnimTrans6, 0);
    callVirt_2C_50(self->mpLayout2, self->mpAnimTrans5, 1);

    // Set anim transform frame
    *(float*)((u8*)self->mpAnimTrans5 + 0x10) = lbl_eu_80668680;

    callVirt_38_0(self->mpLayout2);

    func_80138078__FUl(6);
}

// func_80235DD8 - state machine (state==9 -> 0xC)
// .text:0x6D0, size 0xAC
void func_80235DD8(CArtsInfo* self) {
    if (self->field_0x44 != 9) return;

    self->field_0x44 = 0xC;
    self->field_0x49 = 0;

    // Set cursor visibility
    func_801D216C(self->mCursor, 0);

    // Bind animations to layout 2
    self->mpLayout2->SetAnimationEnable(self->mpAnimTrans5, false);
    self->mpLayout2->SetAnimationEnable(self->mpAnimTrans6, true);

    // Set anim transform frame
    self->mpAnimTrans6->SetFrame(lbl_eu_80668680);

    self->mpLayout2->Animate(0);
}

// func_80235E84 - set field_0x54
// .text:0x77C, size 0x8
void func_80235E84(CArtsInfo* self, u8 val) { self->field_0x54 = val; }

// func_80235E8C - set field_0x55
// .text:0x784, size 0x8
void func_80235E8C(CArtsInfo* self, u8 val) { self->field_0x55 = val; }

// func_80235E94 - set field_0x56
// .text:0x78C, size 0x8
void func_80235E94(CArtsInfo* self, u8 val) { self->field_0x56 = val; }

// func_80235E9C - set field_0x58
// .text:0x794, size 0x8
void func_80235E9C(CArtsInfo* self, u16 val) { self->field_0x58 = val; }

// func_80235EA4 - call func_80236508, check fields, call func_802369C0
// .text:0x79C, size 0x4C
void func_80235EA4(CArtsInfo* self) {
    func_80236508(self);

    if (self->field_0x54 != 0 && self->field_0x55 != 0) {
        func_802369C0(self);
    }
}

// func_80235EF0 - decrement field_0x5A, tail-call func_80236CF4
// .text:0x7E8, size 0x24
void func_80235EF0(CArtsInfo* self) {
    // Retail keeps the byte arithmetic unsigned: lbz (no extsb), subi, stb,
    // then masks/sign-extends only for the clamp test.
    u8 val = (u8)self->field_0x5A - 1;
    self->field_0x5A = val;
    if ((s8)val < 0) self->field_0x5A = 1;
    func_80236CF4(self);
}

// func_80235F14 - increment field_0x5A, tail-call func_80236CF4
// .text:0x80C, size 0x28
void func_80235F14(CArtsInfo* self) {
    u8 val = (u8)self->field_0x5A + 1;
    self->field_0x5A = val;
    if ((s8)val > 1) self->field_0x5A = 0;
    func_80236CF4(self);
}

// func_80235F3C - check if field_0x5A is zero
// .text:0x834, size 0x14
u32 func_80235F3C(CArtsInfo* self) {
    s8 val = self->field_0x5A;
    return (val == 0) ? 1 : 0;
}

// func_80235F50 - signed (field_0x44 >= 6) via subfc/adde idiom
// .text:0x848, size 0x1C
u32 func_80235F50(CArtsInfo* self) {
    int state = self->field_0x44;
    return (state >= 6) ? 1 : 0;
}

// func_80235F6C - animation state 0
// .text:0x864, size 0xB4
void func_80235F6C(CArtsInfo* self) {
    float f = lbl_eu_80668684;
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(self->mpAnimTrans1, f) == 0) return;

    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans3, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans4, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans1, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans2, 1);

    self->field_0x44 = 2;
}

// func_80236020 - animation state 1
// .text:0x918, size 0x4C
void func_80236020(CArtsInfo* self) {
    float f = lbl_eu_80668684;
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(self->mpAnimTrans2, f)) {
        self->field_0x44 = 3;
        self->field_0x49 = 1;
    }
}

// func_8023606C - animation state 2
// .text:0x964, size 0xB4
void func_8023606C(CArtsInfo* self) {
    if (func_80137510(self->mpAnimTrans2, lbl_eu_80668684) == 0) return;

    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans2, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans3, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans4, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans1, true);

    self->field_0x44 = 5;
}

// func_80236120 - animation state 3
// .text:0xA18, size 0x4C
void func_80236120(CArtsInfo* self) {
    if (func_80137510(self->mpAnimTrans1, lbl_eu_80668684) == 0) return;

    self->field_0x49 = 1;
    self->field_0x44 = 0;
}

// func_8023616C - animation state 4
// .text:0xA64, size 0xB4
void func_8023616C(CArtsInfo* self) {
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(self->mpAnimTrans3, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 7;

    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans2, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans3, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans1, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans4, 1);
}

// func_80236220 - animation state 5
// .text:0xB18, size 0xB4
void func_80236220(CArtsInfo* self) {
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(self->mpAnimTrans4, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 8;

    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans2, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans3, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans1, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans4, 1);
}

// func_802362D4 - animation state 6
// .text:0xBCC, size 0x60
void func_802362D4(CArtsInfo* self) {
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(self->mpAnimTrans5, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 9;
    self->field_0x49 = 1;
    func_801D216C(self->mCursor, 1);
    func_80236CF4(self);
}

// func_80236334 - animation state 7
// .text:0xC2C, size 0xD4
void func_80236334(CArtsInfo* self) {
    u32 done1 = func_80137510(self->mpAnimTrans4, lbl_eu_80668684);
    u32 done2 = func_80137510(self->mpAnimTrans5, lbl_eu_80668684);

    if (done1 == 0) return;
    if (done2 == 0) return;

    self->field_0x44 = 0xB;

    // Real member calls so MWCC emits the retail r12 vtable dispatch
    // (vtable[0x2C] = Layout::SetAnimationEnable).
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans2, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans1, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans4, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans3, true);
}

// func_80236408 - animation state 8
// .text:0xD00, size 0x4C
void func_80236408(CArtsInfo* self) {
    if (func_80137510(self->mpAnimTrans3, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 3;
    self->field_0x49 = 1;
}

// func_80236454 - animation state 9
// .text:0xD4C, size 0xB4
void func_80236454(CArtsInfo* self) {
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(self->mpAnimTrans6, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 0xB;

    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans2, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans1, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans4, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans3, 1);
}

// func_80236508 - large layout setup (bind all pane animations)
// .text:0xE00, size 0x4B8
void func_80236508(CArtsInfo* self) {
    // Bind animations for all named panes on layout 1
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x50, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x59, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x64, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x72, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x80, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x8B, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x96, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0xA1, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0xAC, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0xB7, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0xC2, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0xCD, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0xD8, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0xE3, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0xEE, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0xFD, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x10C, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x11B, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x12A, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x139, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x148, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x157, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x166, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x175, lbl_eu_8050B00C + 0x58, 0);

    // Bind animations for all named panes on layout 2
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x50, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x59, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x64, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x72, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x80, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x8B, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x96, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0xA1, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0xAC, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0xB7, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0xC2, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0xCD, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0xD8, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0xE3, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0xEE, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0xFD, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x10C, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x11B, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x12A, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x139, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x148, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x157, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x166, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x175, lbl_eu_8050B00C + 0x58, 0);

    // Set text rendering parameters for specific panes on layout 2
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0xEE, &lbl_eu_80664748, &lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0xFD, &lbl_eu_80664748, &lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x10C, &lbl_eu_80664748, &lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x11B, &lbl_eu_80664748, &lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x12A, &lbl_eu_80664748, &lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x139, &lbl_eu_80664748, &lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x148, &lbl_eu_80664748, &lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x157, &lbl_eu_80664748, &lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x166, &lbl_eu_80664748, &lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x175, &lbl_eu_80664748, &lbl_eu_80664750);
}

void func_802369C0(){}

// func_80236CF4 - cursor name display. Formats the current cursor slot name
// (field_0x5A + 1) and binds it to the pane found by name on layout 1, then
// sends the string to the cursor's message virtual. Early-exits when the
// cursor slot is negative (cursor hidden).
// noinline: the state handlers (func_80235EF0/35F14/362D4) tail-call the
// retail symbol; inlining would balloon their bodies.
// optimize_for_size merges the r29/r30/r31 saves into the retail stmw r29
// prologue.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80236CF4(CArtsInfo* self) {
    char buf[0x20]; // sprintf at +0x18
    nw4r::math::VEC3 pos; // func_80137924 output at +0x8
    s8 idx = self->field_0x5A;
    if (idx < 0) return;

    sprintf(buf, lbl_eu_8050B00C + 0x1b8, idx + 1);
    nw4r::lyt::Pane* pane1 = self->mpLayout1->GetRootPane()->FindPaneByName(buf, true);
    nw4r::lyt::Pane* pane2 = self->mpLayout1->GetRootPane()->FindPaneByName(lbl_eu_8050B00C + 0x1c5, true);
    func_80137924(&pos, pane1, pane2, self->mpLayout1->GetRootPane());
    reinterpret_cast<CArtsInfoCurView*>(&self->mCursor[0])->vf04(&pos);
}
#pragma optimize_for_size off

// func_80236DB8 - fetch stat block via vtable[0x224] dispatch, return s16 id
// .text:0x16B0, size 0x38
s16 func_80236DB8(CArtsInfo* self) {
    CArtsCharData* obj = (CArtsCharData*)func_8009EC9C(self->field_0x54);
    CArtsStatsV* s = (CArtsStatsV*)&obj->stats;
    CArtsStatBlock* st = s->getStatBlock();
    return st->field_0x1C;
}

// func_80236DF0 - same dispatch, return the id at +0x20. Retail treats the
// result as a full-width int (callers use it without re-sign-extending:
// func_8023A2D8 shifts it directly), so the return type is int.
// .text:0x16E8, size 0x38
// noinline + extern "C": the sprintf-family callers (func_8023A2D8) must emit
// a bl to the retail (unmangled) symbol, not an inlined vtable dispatch.
extern "C" __declspec(noinline) int func_80236DF0(CArtsInfo* self) {
    CArtsCharData* obj = (CArtsCharData*)func_8009EC9C(self->field_0x54);
    CArtsStatsV* s = (CArtsStatsV*)&obj->stats;
    CArtsStatBlock* st = s->getStatBlock();
    return st->field_0x20;
}

// func_80236E28 - same dispatch, return (int)float at +0x10
// .text:0x1720, size 0x44
int func_80236E28(CArtsInfo* self) {
    CArtsCharData* obj = (CArtsCharData*)func_8009EC9C(self->field_0x54);
    CArtsStatsV* s = (CArtsStatsV*)&obj->stats;
    CArtsStatBlock* st = s->getStatBlock();
    return (int)st->field_0x10;
}

// func_80236E6C - arts equipment/level lookup. Reads the character's six
// s16 slots (weapon + five stat ids at +0x26/+0x1C..+0x24), and for each
// slot type (a byte of the 0x80668688/0x8066868C flags) fetches the item
// list entry; if the entry's slots contain the requested item id, the
// matching slot's value accumulates into the result. Finally the result is
// capped at the row value from the 0x1d4 table (signed compare).
extern "C" __declspec(noinline) int func_80236E6C(CArtsInfo* self, int arg2) {
    CArtsCharData* obj = (CArtsCharData*)func_8009EC9C(self->field_0x54);
    int result = 0;
    // Declaration order drives saved-reg allocation (first -> highest):
    // retail keeps the flags base in r28, ids base in r27 (result in r31).
    CArtsSlotFlags flags;
    s16 ids[6];
    ids[0] = obj->field_0x26;
    ids[1] = obj->field_0x1C;
    ids[2] = obj->field_0x1E;
    ids[3] = obj->field_0x20;
    ids[4] = obj->field_0x22;
    ids[5] = obj->field_0x24;
    flags.w.field_0x0 = lbl_eu_80668688;
    flags.w.field_0x4 = lbl_eu_8066868C;

    for (u8 i = 0; i < 6; i++) {
        if (ids[i] == -1) continue;
        CArtsInfoListEntry* e = func_80157C4C(flags.b[i]);
        if (e == 0) continue;
        if (e->field_0x0 == 0) continue;
        // Retail re-fetches the impl object at every dispatch (5 bl's); a
        // hoisted local would drop two of them.
        u8 count = CItem_initItemImplInstances(e)->vf12(e);
        for (u8 j = 0; j < count; j++) {
            s16 id = CItem_initItemImplInstances(e)->vf16(e, j);
            if (id != -1) {
                CArtsInfoListEntry* e3 = func_80157C4C(3);
                if (e3 != 0 && e3->field_0x0 != 0) {
                    if ((u32)arg2 == CItem_initItemImplInstances(e3)->vf21(e3)) {
                        result += CItem_initItemImplInstances(e3)->vf36(e3);
                    }
                }
            } else {
                CArtsItemInfo* p = CItem_initItemImplInstances(e)->vf11(e, j);
                if (p != 0) {
                    if ((u32)arg2 == ((p->field_0x4 >> 4) & 0xfff)) {
                        result += (s16)((p->field_0x0 >> 10) & 0x7ff);
                    }
                }
            }
        }
    }
    const char* base = lbl_eu_8050B00C;
    u16 t = (u16)func_80136254((const void*)lbl_eu_806640D8, base + 0x1d4, arg2);
    if (t < result) result = t;
    return result;
}

// func_80237050 - arts skill lookup (flag 0x5C)
// .text:0x18E0, size 0x58
// Walks the character-data +0x3534 object: func_8026178C tests whether the
// skill is reachable, func_8025FB10 returns its summed value (see
// code_8025FB10.cpp).
// extern "C" + noinline: retail callers (func_80237A0C) emit a bl to the
// unmangled out-of-line symbol; without both, MWCC mangles the call-site
// reloc and -inline auto expands the body inline. int return: the caller's
// (float) conversion uses the signed xoris magic (retail codegen).
extern "C" __declspec(noinline) int func_80237050(CArtsInfo* self) {
    u8* obj = (u8*)func_8009EC9C(self->field_0x54) + 0x3534;
    if (func_8026178C(obj, 0x5C) != 0) {
        return func_8025FB10(obj, 0x5C);
    }
    return 0;
}

// func_802370A8 - arts skill lookup (flag 0x2D)
// .text:0x1938, size 0x58
// extern "C": the retail symbol is unmangled, and func_80237238 must emit a
// bl to it (noinline + C linkage keeps the call-site reloc name retail-identical).
extern "C" __declspec(noinline) u32 func_802370A8(CArtsInfo* self) {
    u8* obj = (u8*)func_8009EC9C(self->field_0x54) + 0x3534;
    if (func_8026178C(obj, 0x2D) != 0) {
        return func_8025FB10(obj, 0x2D);
    }
    return 0;
}

// func_80237100 - arts list lookup: fetch two table entries keyed by arg3
// (or field_0x55 when arg3 is 0) and combine them into a grid offset:
// u16 first-entry + u8 second-entry * (arg2 - 1). The ternaries stay int so
// the u8 mask is emitted at the call sites (retail: mr r0,r30 / clrlwi).
#pragma optimize_for_size on
// extern "C" + noinline: retail callers (func_802397F4) emit a bl to the
// unmangled out-of-line symbol; without both, MWCC mangles the call-site
// reloc and -inline auto expands the body inline. u8 params: retail call
// sites mask the level+1 argument (clrlwi 24) before the call.
extern "C" __declspec(noinline) int func_80237100(CArtsInfo* self, u8 arg2, u8 arg3) {
    int v1 = (arg3 != 0) ? arg3 : self->field_0x55;
    u16 a = func_8013606C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x1d8, v1);
    int v2 = (arg3 != 0) ? arg3 : self->field_0x55;
    u8 b = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x1de, v2);
    return a + b * (arg2 - 1);
}
#pragma optimize_for_size off

// func_8023719C - arts list lookup: same shape as func_80237100 but with
// different string-table offsets (0x1e8/0x1ee).
#pragma optimize_for_size on
// extern "C" + noinline: see func_80237100 (u8 params).
extern "C" __declspec(noinline) int func_8023719C(CArtsInfo* self, u8 arg2, u8 arg3) {
    int v1 = (arg3 != 0) ? arg3 : self->field_0x55;
    u16 a = func_8013606C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x1e8, v1);
    int v2 = (arg3 != 0) ? arg3 : self->field_0x55;
    u8 b = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x1ee, v2);
    return a + b * (arg2 - 1);
}
#pragma optimize_for_size off

// func_80237238 - arts info damage/level helper. Reads the character's weapon
// id (+0x26 of the func_8009EC9C data), finds the entry whose first word's
// top 12 bits match it (func_80157C4C/func_80139358), then looks up the
// weapon's name row (func_80136254) and a flag byte (func_801361E8). When
// flag bit 4 is set, computes a capped HP value = scale1 * (nameRow * base),
// clamped to 999. The final result is
// scale2 * ((hp & 0xffff) * (skill + artLevel + 100)) masked to 16 bits.
// Both int->float conversions use MWCC's s32 magic (xoris + lbl_eu_80668698).
// Single shared fail label (return 0) at the bottom, matching retail's block
// layout (all guards jump forward to it).
int func_80237238(CArtsInfo* self) {
    CArtsCharData* obj = (CArtsCharData*)func_8009EC9C(self->field_0x54);
    if (obj->field_0x26 == -1) goto fail;
    CArtsInfoListEntry* e = func_80157C4C(2);
    if (e == 0) goto fail;
    u32 v0 = e->field_0x0;
    if (v0 == 0) goto fail;
    u16 id = func_80139358(v0 >> 20);
    int hp = (int)func_80136254((const void*)lbl_eu_806640F4, lbl_eu_8050B00C + 0x1f8, id);
    u8 b = (u8)func_801361E8(lbl_eu_806640F4, lbl_eu_8050B00C + 0x200, id);
    if ((b & 4) != 0) {
        u16 base = (u16)func_800A082C(obj);
        hp = (int)(lbl_eu_80668690 * (float)(int)((hp & 0xffff) * base));
        if ((hp & 0xffffu) >= 0x3e7u) hp = 0x3e7;  // unsigned cmpli, clamp 999
    }
    // Both lookups inline: MWCC evaluates + right-to-left, so func_802370A8
    // (rightmost) runs first and its result survives in r31 like retail.
    return (int)(lbl_eu_80668694 * (float)(int)((hp & 0xffff) * ((int)func_80236E6C(self, 0x52) + (int)func_802370A8(self) + 100))) & 0xffff;
fail:
    return 0;
}

// func_80237394 - arts info damage/level helper. Same shape as
// func_80237238 but the name-row string offset is 0x205 and the HP
// scale factor is lbl_eu_806686A0.
int func_80237394(CArtsInfo* self) {
    CArtsCharData* obj = (CArtsCharData*)func_8009EC9C(self->field_0x54);
    s16 weapon = obj->field_0x26;
    if (weapon == -1) goto fail;
    CArtsInfoListEntry* e = func_80157C4C(2);
    if (e == 0) goto fail;
    u32 v0 = e->field_0x0;
    if (v0 == 0) goto fail;
    u16 id = func_80139358(v0 >> 20);
    int hp = (int)func_80136254((const void*)lbl_eu_806640F4, lbl_eu_8050B00C + 0x205, id);
    u8 b = (u8)func_801361E8(lbl_eu_806640F4, lbl_eu_8050B00C + 0x200, id);
    if ((b & 4) != 0) {
        u16 base = (u16)func_800A082C(obj);
        hp = (int)(lbl_eu_806686A0 * (float)(int)((hp & 0xffff) * base));
        if ((hp & 0xffffu) >= 0x3e7u) hp = 0x3e7;  // unsigned cmpli, clamp 999
    }
    // Both lookups inline: MWCC evaluates + right-to-left, so func_802370A8
    // (rightmost) runs first and its result survives in r31 like retail.
    return (int)(lbl_eu_80668694 * (float)(int)((hp & 0xffff) * ((int)func_80236E6C(self, 0x53) + (int)func_802370A8(self) + 100))) & 0xffff;
fail:
    return 0;
}

// func_802374F0 - arts skill string lookup
// .text:0x1E00, size 0x5C
// func_801361E8 finds the row for field_0x50 in the lbl_eu_8050B00C+0x20C
// list; if present, func_80136190 formats the string (3rd arg = the row's
// byte index, which retail keeps in r5 from the clrlwi mask). Result lives
// in a local so MWCC hoists `li r30, 0` before the call and returns it via
// r30 (same shape as func_8023754C).
#pragma optimize_for_size on
extern "C" __declspec(noinline) char* func_802374F0(CArtsInfo* self, u32 arg2) {
    char* result = 0;
    u8 r = func_801361E8((u32)self->field_0x50, lbl_eu_8050B00C + 0x20C, arg2);
    if (r != 0) {
        result = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3D, r);
    }
    return result;
}
#pragma optimize_for_size off

// func_8023754C - arts skill string lookup (same shape as func_802374F0,
// different string-table offset 0x212). result lives in a local so MWCC
// hoists `li r30, 0` before the call and returns it via r30.
#pragma optimize_for_size on
extern "C" __declspec(noinline) char* func_8023754C(CArtsInfo* self, u32 arg2) {
    char* result = 0;
    u8 r = func_801361E8((u32)self->field_0x50, lbl_eu_8050B00C + 0x212, arg2);
    if (r != 0) {
        result = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3D, r);
    }
    return result;
}
#pragma optimize_for_size off

void func_802375A8(){}

// func_80237A0C - arts info text update. Formats a computed percentage
// (base value scaled by the arts-skill lookups func_80236E6C(0x2d) and
// func_80237050) into buf1, post-processes it (func_eu_80136F90 +
// func_eu_8023D490), then pushes it onto both layouts (layout 2 only when
// field_0x56 < 0xA). The base is a u8-keyed lookup scaled by
// lbl_eu_806686A4 when non-zero; the skill values are s32->float converted
// with MWCC's xoris magic.
void func_80237A0C(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    u8 r = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x22d, self->field_0x55);
    float d = (float)r;
    if (r != 0) {
        d = d * lbl_eu_806686A4;
    }
    // f = d - scale2 * (d * (float)x) - scale2 * (d * (float)y) via fnmsubs
    // (retail: fmuls d*(float)x; fnmsubs f30, scale2, f1, f31).
    float t = d * (float)func_80236E6C(self, 0x2d);
    float f = d - lbl_eu_80668694 * t;
    f -= lbl_eu_80668694 * (d * (float)func_80237050(self));
    sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, f, s2);
    func_eu_80136F90(buf1);
    func_eu_8023D490(self, arg2, buf1);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

void func_80237B88(){}

// func_80237D58 - arts info text update. Same shape as func_8023A148 but the
// lookup is func_801360CC (s8 result): the value is abs()'d and narrowed back
// to s8 (retail: extsb after the abs call) for the sprintf vararg.
void func_80237D58(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    u32 a2 = arg2;
    char* s1 = func_802374F0(self, a2);
    char* s2 = func_8023754C(self, a2);
    s8 r = func_801360CC(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x263, self->field_0x55);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, (s8)abs(r), s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

void func_80237E24(){}

void func_80238038(){}

void func_80238298(){}

void func_802384F4(){}

void func_80238904(){}

// func_80239030 - arts info text update. Same shape as func_8023AF60 but the
// two lookups' results are combined into a grid offset (func_80237100
// pattern: first-entry + second-entry * (level - 1)), formatted, then on
// level-up the offset for the next level is formatted on layout 2 and the
// colour-pair helper runs when the two offsets differ.
void func_80239030(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    u8 v1 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    u8 v2 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
    u32 cur = v1 + v2 * (self->field_0x56 - 1);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, cur, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = v1 + v2 * self->field_0x56;
        sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, nxt, s2);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            func_80139A18(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

void func_8023916C(){}

void func_8023939C(){}

void func_8023959C(){}

// func_802397F4 - arts info text update. Same shape as func_8023B12C: the
// two string lookups are discarded and the formatted string comes from
// func_80136190(0x32, 0x3d, 0x52), but the two grid offsets come from the
// func_80237100/8023719C lookups keyed on the current arts level. On level-up
// (< 0xA) the offsets for level+1 are formatted on layout 2 and the
// colour-pair helper always runs.
void func_802397F4(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    // Declaration order drives saved-reg allocation (first -> highest):
    // retail keeps level in r30, grid2 in r29, grid1 in r28 (sharing arg2).
    // level is assigned late (after sprintf1) so the addi lands after the
    // first sprintf like retail's in-place addi r30, r30, 2.
    int level;
    u32 grid2;
    u32 grid1;
    func_802374F0(self, arg2);
    func_8023754C(self, arg2);
    grid1 = func_80237100(self, self->field_0x56, 0);
    grid2 = func_8023719C(self, self->field_0x56, 0);
    char* str = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
    sprintf(buf1, lbl_eu_8050B00C + 0x282, grid1, str, grid2);
    level = arg3 + 2;
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 g1n = func_80237100(self, self->field_0x56 + 1, 0);
        u32 g2n = func_8023719C(self, self->field_0x56 + 1, 0);
        char* str2 = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
        sprintf(buf1, lbl_eu_8050B00C + 0x282, g1n, str2, g2n);
        sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        func_80139A18(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
    }
}

// func_80239964 - arts info text update. Byte-identical body to
// func_80239030 (same lookups, same grid-offset formatting, same level-up
// branch), kept as a separate retail symbol.
void func_80239964(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    u8 v1 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    u8 v2 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
    u32 cur = v1 + v2 * (self->field_0x56 - 1);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, cur, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = v1 + v2 * self->field_0x56;
        sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, nxt, s2);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            func_80139A18(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_80239AA0 - arts info text update. Byte-identical body to
// func_80239030 (same lookups, same grid-offset formatting, same level-up
// branch), kept as a separate retail symbol.
void func_80239AA0(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    u8 v1 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    u8 v2 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
    u32 cur = v1 + v2 * (self->field_0x56 - 1);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, cur, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = v1 + v2 * self->field_0x56;
        sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, nxt, s2);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            func_80139A18(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_80239BDC - arts info text update. Same shape as func_80239030 but the
// grid offset is scaled by 10 (mulli 0xa) before formatting: the value is the
// arts AP cost rather than a raw grid index.
void func_80239BDC(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    u8 v1 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    u8 v2 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
    u32 cur = (v1 + v2 * (self->field_0x56 - 1)) * 10;
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, cur, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = (v1 + v2 * self->field_0x56) * 10;
        sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, nxt, s2);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            func_80139A18(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

void func_80239D20(){}

// func_80239EFC - arts info text update. Formats the arts skill name and a
// byte-keyed table value (func_8013600C) into buf1, the level string into
// buf2, then pushes both onto the layouts (layout 2 only when field_0x56
// < 0xA). Same shape as func_8023A55C but the 3rd sprintf arg is a lookup.
void func_80239EFC(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    u32 a2 = arg2;
    char* s1 = func_802374F0(self, a2);
    char* s2 = func_8023754C(self, a2);
    u8 r = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, r, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_80239FC4 - arts info text update. Same shape as func_80239EFC (u8
// lookup at 0x2ab) but the displayed value is a percentage:
// scale * level + base (MWCC u32->float conversions via the 2^52 magic),
// rounded to unsigned. On level-up the level+1 percentage is formatted on
// layout 2 and the colour-pair helper runs when the two values differ.
void func_80239FC4(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    u8 r = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x2ab, self->field_0x55);
    u32 cur = (u32)(lbl_eu_806686BC * (float)self->field_0x56 + (float)r);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, cur, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = (u32)(lbl_eu_806686BC * (float)(self->field_0x56 + 1) + (float)r);
        sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, nxt, s2);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            func_80139A18(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_8023A148 - arts info text update. Same shape as func_80239EFC but
// uses the s16-keyed lookup func_80136130 (result sign-extended at the
// sprintf call site).
void func_8023A148(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    u32 a2 = arg2;
    char* s1 = func_802374F0(self, a2);
    char* s2 = func_8023754C(self, a2);
    s16 r = func_80136130(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x2b3, self->field_0x55);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, r, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_8023A210 - arts info text update. Same shape as func_8023A148
// (s16-keyed lookup func_80136130, result sign-extended at the sprintf call
// site via the s16 vararg promotion).
void func_8023A210(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    u32 a2 = arg2;
    char* s1 = func_802374F0(self, a2);
    char* s2 = func_8023754C(self, a2);
    s16 r = func_80136130(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x2b3, self->field_0x55);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, r, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_8023A2D8 - arts info text update. Formats the arts skill name and the
// doubled stat id (func_80236DF0) into buf1, the level string into buf2,
// then pushes both onto the layouts (layout 2 only when field_0x56 < 0xA).
void func_8023A2D8(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    u32 a2 = arg2;
    char* s1 = func_802374F0(self, a2);
    char* s2 = func_8023754C(self, a2);
    int id = func_80236DF0(self);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, id << 1, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_8023A398 - arts info text update. Same shape as func_80239EFC
// (u8-keyed lookup func_8013600C at 0x289, zero-extended at the sprintf
// call site via the u8 vararg promotion).
void func_8023A398(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    u32 a2 = arg2;
    char* s1 = func_802374F0(self, a2);
    char* s2 = func_8023754C(self, a2);
    u8 r = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, r, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_8023A460 - arts info text update. Same shape as func_8023B280 but the
// converted double is scaled by lbl_eu_806686A4 when the s16 lookup is
// non-zero (retail: extsh. branch + lfs/fmuls).
void func_8023A460(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    s16 r = func_80136130(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x2b3, self->field_0x55);
    // MWCC s32->f32 magic (retail: extsh + 0x4330/xoris/lfd/fsubs, no frsp).
    float d = (float)r;
    if (r != 0) {
        d = d * lbl_eu_806686A4;
    }
    sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, d, s2);
    func_eu_80136F90(buf1);
    func_eu_8023D490(self, arg2, buf1);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_8023A55C - arts info text update. Formats two strings (arts skill
// names + level) and pushes them onto both layouts (layout 2 only when
// field_0x56 < 0xA).
#pragma optimize_for_size on
void func_8023A55C(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    u32 a2 = arg2;
    char* s1 = func_802374F0(self, a2);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, 5, func_8023754C(self, a2));
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}
#pragma optimize_for_size off

// func_8023A60C - arts info text update. Formats the skill name, a double
// constant and the second skill string into buf1, post-processes it
// (func_eu_80136F90) and hands it to func_eu_8023D490 before the usual
// level-string + dual-layout push (layout 2 only when field_0x56 < 0xA).
void func_8023A60C(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, lbl_eu_806686C0, s2);
    func_eu_80136F90(buf1);
    func_eu_8023D490(self, arg2, buf1);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_8023A6BC - same shape as func_8023A55C with level constant 0x19.
#pragma optimize_for_size on
void func_8023A6BC(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, 25, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}
#pragma optimize_for_size off

// func_8023A76C - arts info text update with fixed level 25 (same shape as
// func_8023A6BC). Formats the arts skill name/level strings and pushes them
// onto both layouts (layout 2 only when field_0x56 < 0xA).
#pragma optimize_for_size on
void func_8023A76C(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, 25, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}
#pragma optimize_for_size off

// func_8023A81C - same shape as func_8023A76C with level constant 50.
#pragma optimize_for_size on
void func_8023A81C(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, 50, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}
#pragma optimize_for_size off

// func_8023A8CC - same shape as func_8023A76C with level constant 100.
#pragma optimize_for_size on
void func_8023A8CC(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, 100, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}
#pragma optimize_for_size off

// func_8023A97C - arts info text update. Same shape as func_8023A60C with
// a different format offset (0x2bb) and double constant (lbl_eu_806686C8).
void func_8023A97C(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    sprintf(buf1, lbl_eu_8050B00C + 0x2bb, s1, lbl_eu_806686C8, s2);
    func_eu_80136F90(buf1);
    func_eu_8023D490(self, arg2, buf1);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_8023AA2C - same shape as func_8023A76C with level constant 50.
#pragma optimize_for_size on
void func_8023AA2C(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, 50, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}
#pragma optimize_for_size off

// func_8023AADC - same shape as func_8023A76C with level constant 2.
#pragma optimize_for_size on
void func_8023AADC(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, 2, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}
#pragma optimize_for_size off

void func_8023AB8C(){}

// func_8023AD5C - arts info text update. Same shape as func_80239EFC with
// the u8-keyed lookup func_8013600C at 0x2ab.
void func_8023AD5C(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    u32 a2 = arg2;
    char* s1 = func_802374F0(self, a2);
    char* s2 = func_8023754C(self, a2);
    u8 r = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x2ab, self->field_0x55);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, r, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_8023AE24 - arts info text update. Byte-identical body to
// func_80239030 / func_80239AA0 (same lookups, same grid-offset formatting,
// same level-up branch), kept as a separate retail symbol.
void func_8023AE24(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    u8 v1 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    u8 v2 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
    u32 cur = v1 + v2 * (self->field_0x56 - 1);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, cur, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = v1 + v2 * self->field_0x56;
        sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, nxt, s2);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            func_80139A18(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_8023AF60 - arts info text update. Two byte-keyed lookups
// (func_8013600C at 0x289/0x29f) are called and their results discarded
// (retail keeps the bl's but never materializes the return values), then the
// arts level (field_0x56) is formatted. On level-up (< 0xA) the level+1
// string replaces buf1 on layout 2 and the colour-pair helper runs when the
// two level values differ.
void func_8023AF60(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    CArtsInfo* s = self;
    char* s1 = func_802374F0(s, arg2);
    char* s2 = func_8023754C(s, arg2);
    func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
    u32 level = self->field_0x56;
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, level, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = self->field_0x56 + 1;
        sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, nxt, s2);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (level != nxt) {
            func_80139A18(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_8023B074 - arts info text update. Same shape as func_8023A60C but the
// vararg double constant is loaded from a float (lfs) pool slot
// (lbl_eu_806686A0).
void func_8023B074(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, lbl_eu_806686A0, s2);
    func_eu_80136F90(buf1);
    func_eu_8023D490(self, arg2, buf1);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_8023B12C - arts info text update. The two string lookups
// (func_802374F0/8023754C) are called but their results discarded; the
// formatted string comes from func_80136190(0x32, 0x3d, 0x52) instead. Same
// grid-offset (v1 + v2*(level-1)) and level-up branch as func_80239030, with
// the format at 0x282 and the level constant 5.
void func_8023B12C(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    func_802374F0(self, arg2);
    func_8023754C(self, arg2);
    u8 v1 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    u8 v2 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
    u32 cur = v1 + v2 * (self->field_0x56 - 1);
    char* str = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
    sprintf(buf1, lbl_eu_8050B00C + 0x282, 5, str, cur);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = v1 + v2 * self->field_0x56;
        char* str2 = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
        sprintf(buf1, lbl_eu_8050B00C + 0x282, 5, str2, nxt);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            func_80139A18(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_8023B280 - arts info text update. Same shape as func_8023A60C but the
// vararg double is the s16-keyed lookup result (func_80136130 at 0x2b3)
// converted via MWCC's s32->f32 magic (retail: extsh + 0x4330/xoris/lfd/fsubs
// trick, no frsp).
void func_8023B280(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    s16 r = func_80136130(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x2b3, self->field_0x55);
    // MWCC s32->f32 magic (retail: extsh + 0x4330/xoris/lfd/fsubs, no frsp).
    float d = (float)r;
    sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, d, s2);
    func_eu_80136F90(buf1);
    func_eu_8023D490(self, arg2, buf1);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_8023B368 - arts info text update. Same shape as func_8023A148 but the
// s16-keyed lookup func_80136130 uses table offset 0x2c4.
void func_8023B368(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    u32 a2 = arg2;
    char* s1 = func_802374F0(self, a2);
    char* s2 = func_8023754C(self, a2);
    s16 r = func_80136130(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x2c4, self->field_0x55);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, r, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_eu_8023D490 - German-language arts text post-processing. Only runs
// when the system language is 5 (German) and the row id for field_0x50 in the
// 0x20c table is 0x4f. Replaces '.' with ',' and skips the second byte of
// double-byte (Shift-JIS) characters so byte counts stay aligned. C linkage
// (retail symbol is unmangled; the extern "C" declaration is in the header).
// optimize_for_size gives the retail stmw r29 prologue for the 3 saved params.
#pragma optimize_for_size on
__declspec(noinline) void func_eu_8023D490(CArtsInfo* self, u32 arg2, char* str) {
    u8 lang = getLanguage__9CDeviceSCFv();
    if (lang != 5) return;

    u8 r = func_801361E8((u32)self->field_0x50, lbl_eu_8050B00C + 0x20c, arg2);
    if (r != 0x4f) return;

    int len = strlen(str);
    int i = 0;
    while (i < len) {
        s8 c = *str;
        if (c == 0x2e) {
            *str = 0x2c; // '.' -> ','
        } else if (c >= 0x81 && c <= 0x9f) {
            i += 1;
            str += 1;
        } else if (c >= 0xe0 && c <= 0xef) {
            i += 1;
            str += 1;
        }
        i += 1;
        str += 1;
    }
}
#pragma optimize_for_size off

void CArtsInfo::setField54(u8 val) { field_0x54 = val; }

void CArtsInfo::setField55(u8 val) { field_0x55 = val; }

void CArtsInfo::setField56(u8 val) { field_0x56 = val; }

void CArtsInfo::setField58(u16 val) { field_0x58 = val; }

u32 CArtsInfo::getField5A() {
    s8 val = field_0x5A;
    u32 result = __cntlzw(val);
    return result >> 5;
}

int CArtsInfo::isField44GE6() {
    int field = field_0x44;
    return field >= 6 ? 1 : 0;
}

void CArtsInfo::initialize() {
    if (mpLayout1 == nullptr) return;
    if (field_0x4C == 0) return;
    if (field_0x50 != 0) {
        field_0x48 = 1;
        field_0x40 = 1;
    }
}

void CArtsInfo::OnFileEvent() {}



// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Static constructor: initialise the arts font/colour small-data entries
// (same shape as CItemBoxInfo's sinit_801EABC4).
void sinit_8023BC8C() {
    func_801D1F9C(&lbl_eu_80664748, 0);
    func_801D1F9C(&lbl_eu_80664750, 0);
    func_801C4B60(&lbl_eu_80664758, 0xff, 0xff, 0xfa, 0);
    func_801C4B60(&lbl_eu_80664760, 0x25, 0x8a, 0xce, 0);
}
