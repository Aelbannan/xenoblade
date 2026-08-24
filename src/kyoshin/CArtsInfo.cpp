// Auto-scaffolded catalog TU for kyoshin/CArtsInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/CArtsInfo.hpp"
#include <nw4r/lyt.h>
#include <stdlib.h>

// Extern declarations for referenced symbols

// String table base (rodata, accessed via sda21 relocation)
extern char lbl_eu_8050B00C[];

// Vtable symbol for CArtsInfo (array form: name decays to its address so the
// ctor stores it directly like retail)
extern void* lbl_eu_80536A88[];

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
extern "C" __declspec(noinline) void func_80236508(CArtsInfo*);
extern "C" void func_802369C0(CArtsInfo*);
extern "C" void func_80236CF4(CArtsInfo*);
extern "C" void func_802375A8(CArtsInfo*, u8, u8);
// Damage/level helpers: extern "C" so the sprintf-family call sites
// (func_80238298 / func_80238038) emit bl to the unmangled retail names.
extern "C" int func_80237238(CArtsInfo*);
extern "C" int func_80237394(CArtsInfo*);
// Stat-id helper: noinline + C linkage so func_80238038 emits a bl to the
// retail (unmangled) symbol instead of an inlined vtable dispatch. The
// definition (later in this TU) inherits the linkage; its body is unchanged.
extern "C" __declspec(noinline) int func_80236DB8(CArtsInfo*);

// Forward declarations for animation state handlers. noinline: the dispatcher
// func_8023587C must emit bl's to them through the retail jump table;
// inlining would erase the dispatch entirely.
__declspec(noinline) void func_80235F6C(CArtsInfo*);
__declspec(noinline) void func_80236020(CArtsInfo*);
__declspec(noinline) void func_8023606C(CArtsInfo*);
__declspec(noinline) void func_80236120(CArtsInfo*);
__declspec(noinline) void func_8023616C(CArtsInfo*);
__declspec(noinline) void func_80236220(CArtsInfo*);
__declspec(noinline) void func_802362D4(CArtsInfo*);
__declspec(noinline) void func_80236334(CArtsInfo*);
__declspec(noinline) void func_80236408(CArtsInfo*);
__declspec(noinline) void func_80236454(CArtsInfo*);

// Manual signed-int -> double conversion (docs/MWCC_PATTERNS.md 7i): build
// the 0x4330000080000000 bit pattern and subtract the shared sdata2 magic so
// the lfd references lbl_eu_80668698 instead of a TU-local pool label.
static double ConvS32ToF64(s32 x) {
    union {
        double d;
        u32 w[2];
    } u;
    // xoris word first, then 0x43300000, or MWCC hoists the lis out of order.
    u.w[1] = (u32)x ^ 0x80000000;
    u.w[0] = 0x43300000;
    return u.d - lbl_eu_80668698;
}


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
// __ct__CArtsInfo - constructor. Retail is a C-linkage global taking `this`
// (MWCC leaves __-prefixed globals unmangled); written free-function so the
// UnkClass_8045F564 member is not auto-constructed twice.
CArtsInfo* __ct__CArtsInfo(CArtsInfo* self) {
    // Set vtable pointer
    *(void**)self = lbl_eu_80536A88;

    // Construct embedded UnkClass_8045F564 member
    __ct__17UnkClass_8045F564Fv(&self->mMemRegion);

    // Initialize all fields to zero/default
    self->field_0x14 = 0;
    self->field_0x18 = 0;
    self->field_0x1C = 0;
    self->mpLayout1 = nullptr;
    self->mpAnimTrans1 = nullptr;
    self->mpAnimTrans2 = nullptr;
    self->mpAnimTrans3 = nullptr;
    self->mpAnimTrans4 = nullptr;
    self->mpLayout2 = nullptr;
    self->mpAnimTrans5 = nullptr;
    self->mpAnimTrans6 = nullptr;
    self->field_0x40 = 0;
    self->field_0x44 = 0;
    self->field_0x48 = 0;
    self->field_0x49 = 1;
    self->field_0x4C = 0;
    self->field_0x50 = 0;
    self->field_0x54 = 0;
    self->field_0x55 = 0;
    self->field_0x56 = 0;
    self->field_0x58 = 0;
    self->field_0x5A = 0;

    // Construct embedded CCur18 cursor
    __ct__CCur18(self->mCursor, 0);
    return self;
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

    // Retail guards the dispatch with a single unsigned cmplwi against 0xC.
    u32 state = self->field_0x44;
    if (state <= 0xC) {
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
        // Cases 0xA-0xC are explicit empty cases so MWCC emits the full
        // 13-entry jump table under the single unsigned <= 0xC guard.
        case 0xA:
        case 0xB:
        case 0xC:
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
__declspec(noinline) void func_802359CC(CArtsInfo* self) {
    func_8003AA8C__5CBdatFUl(2);

    func_801390E0__FPP11CFileHandle(&self->field_0x14);
    func_801390E0__FPP11CFileHandle(&self->field_0x18);

    self->field_0x40 = 0;

    // Doubled-beq shape: outer guard, inner guard around the deleting-dtor
    // virtual (+0x08, flag=1), unconditional-clear inside the outer guard.
    if (self->mpLayout1 != nullptr) {
        if (self->mpLayout1 != nullptr) {
            ((CArtsInfoLytView*)self->mpLayout1)->Destroy(1);
        }
        self->mpLayout1 = nullptr;
    }

    if (self->mpLayout2 != nullptr) {
        if (self->mpLayout2 != nullptr) {
            ((CArtsInfoLytView*)self->mpLayout2)->Destroy(1);
        }
        self->mpLayout2 = nullptr;
    }

    // Cleanup arc resource accessor (field holds the pointer by value)
    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor((void*)self->field_0x1C);

    // Call virtual cleanup on cursor (+0x0C)
    ((CArtsInfoCurView*)self->mCursor)->vf03();

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
// optimize_for_size gives the retail _savegpr_28/_restgpr_28 prologue; real
// member calls give the retail r12 vtable dispatches (vtable+0x2C =
// SetAnimationEnable, vtable+0x38 = Animate).
#pragma optimize_for_size on
void func_80235AE0(CArtsInfo* self) {
    if (self->field_0x44 != 3) return;

    self->field_0x44 = 6;
    self->field_0x49 = 0;

    // Bind animations to layout 1
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans2, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans4, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans3, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans1, true);

    // Reset anim transform frame (retail stores 0.0f straight to +0x10)
    *(float*)((u8*)self->mpAnimTrans4 + 0x10) = lbl_eu_80668680;

    self->mpLayout1->Animate(0);

    // Bind animations to layout 1 again
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans2, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans4, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans1, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans3, true);

    // Reset anim transform frame
    *(float*)((u8*)self->mpAnimTrans3 + 0x10) = lbl_eu_80668680;

    self->mpLayout1->Animate(0);

    // Bind animations to layout 2
    self->mpLayout2->SetAnimationEnable(self->mpAnimTrans6, false);
    self->mpLayout2->SetAnimationEnable(self->mpAnimTrans5, true);

    // Reset anim transform frame
    *(float*)((u8*)self->mpAnimTrans5 + 0x10) = lbl_eu_80668680;

    self->mpLayout2->Animate(0);

    // Reset cursor state
    self->field_0x5A = 0;

    // Update layout elements with arts data: fetch the character data, then
    // read the display value through the stats sub-object's vtable+0x200
    // virtual (user virtual #126).
    CArtsCharData* obj = (CArtsCharData*)func_8009EC9C(self->field_0x54);
    char* base = lbl_eu_8050B00C;
    char* str1 = func_80136190(base + 0x32, base + 0x3D, 0x18);
    int dispVal = ((CArtsStatsDisp*)&obj->stats)->vf126();

    char buf[32];
    sprintf(buf, base + 0x42, dispVal, str1);

    func_80136A1C(self->mpLayout1, base + 0x47, buf, 0);

    func_80138078__FUl(0x6D);
}
#pragma optimize_for_size off

// func_80235D24 - state machine (state==9 -> 0xA)
// .text:0x61C, size 0xB4
void func_80235D24(CArtsInfo* self) {
    if (self->field_0x44 != 9) return;

    self->field_0x44 = 0xA;
    self->field_0x49 = 0;

    // Set cursor visibility
    func_801D216C(self->mCursor, 0);

    // Real member calls so MWCC emits the retail r12 vtable dispatch
    // (vtable[0x2C] = SetAnimationEnable, vtable[0x38] = Animate).
    self->mpLayout2->SetAnimationEnable(self->mpAnimTrans6, false);
    self->mpLayout2->SetAnimationEnable(self->mpAnimTrans5, true);

    // Set anim transform frame
    *(float*)((u8*)self->mpAnimTrans5 + 0x10) = lbl_eu_80668680;

    self->mpLayout2->Animate(0);

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

    // Real member calls so MWCC emits the retail r12 vtable dispatch.
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans3, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans4, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans1, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans2, true);

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

    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans2, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans3, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans1, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans4, true);
}

// func_80236220 - animation state 5
// .text:0xB18, size 0xB4
void func_80236220(CArtsInfo* self) {
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(self->mpAnimTrans4, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 8;

    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans2, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans3, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans1, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans4, true);
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

    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans2, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans1, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans4, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans3, true);
}

// func_80236508 - large layout setup (bind all pane animations)
// noinline + extern "C": func_80235EA4 (and OnFileEvent) tail-call the bare
// retail symbol; inlining would balloon their bodies. optimize_for_size
// merges the r30/r31 saves into the retail stmw r30 prologue (plain -O4,p
// emits two separate stw's, +8 bytes).
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80236508(CArtsInfo* self) {
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
#pragma optimize_for_size off

// func_802369C0 - arts info full layout refresh. Formats the arts name/lv
// strings and the colour/flag rows, then binds the row strings to the named
// panes on both layouts. The trailing loop walks arts rows 1..10 (slot 0x1b2
// names) and grows the arts list (func_802375A8) for each row that has a
// non-empty entry.
// noinline: func_80235EA4 (FULL_MATCH) tail-calls the retail symbol; inlining
// would balloon its body. optimize_for_size gives the retail stmw r25
// prologue for the 7 saved GPRs.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_802369C0(CArtsInfo* self) {
    char buf[32]; // sprintf at +0x8
    char* str1 = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x18);
    sprintf(buf, lbl_eu_8050B00C + 0x42, self->field_0x58, str1);
    func_80136A1C(self->mpLayout1, lbl_eu_8050B00C + 0x184, buf, 0);

    char* str2 = func_80136190(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x3d, self->field_0x55);
    char* str3 = 0;
    if (self->field_0x56 != 0) {
        str3 = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, self->field_0x56 + 0x1e);
    }
    sprintf(buf, lbl_eu_8050B00C + 0x193, str2, str3);
    func_80136A1C(self->mpLayout1, lbl_eu_8050B00C + 0x50, buf, 0);

    char* str4 = 0;
    if (self->field_0x56 < 0xA) {
        if (self->field_0x56 != 0) {
            str4 = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, self->field_0x56 + 0x1f);
        }
        sprintf(buf, lbl_eu_8050B00C + 0x193, str2, str4);
        func_80136A1C(self->mpLayout2, lbl_eu_8050B00C + 0x50, buf, 0);
    }

    char* s = func_8013639C((const void*)self->field_0x4C, lbl_eu_8050B00C + 0x198, self->field_0x55);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x59, s, 0);
    if (self->field_0x56 < 0xA) {
        func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x59, s, 0);
    }

    u8 f1 = func_801361E8(self->field_0x4C, lbl_eu_8050B00C + 0x19c, self->field_0x55);
    u8 f2 = func_801361E8(self->field_0x4C, lbl_eu_8050B00C + 0x1a1, self->field_0x55);
    u8 f3 = func_801361E8(self->field_0x4C, lbl_eu_8050B00C + 0x1a6, self->field_0x55);
    char* p1 = 0;
    char* p2 = 0;
    char* p3 = 0;
    if (f1 != 0) p1 = func_8013639C((const void*)self->field_0x50, lbl_eu_8050B00C + 0x1ab, f1);
    if (f2 != 0) p2 = func_8013639C((const void*)self->field_0x50, lbl_eu_8050B00C + 0x1ab, f2);
    if (f3 != 0) p3 = func_8013639C((const void*)self->field_0x50, lbl_eu_8050B00C + 0x1ab, f3);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x64, p1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x64, p1, 0);
    }
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x80, p2, 0);
    if (self->field_0x56 < 0xA) {
        func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x80, p2, 0);
    }
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x8b, p3, 0);
    if (self->field_0x56 < 0xA) {
        func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x8b, p3, 0);
    }

    // Loop counters: full-width ints narrowed to u8 at the call sites
    // (retail clrlwi's the sprintf vararg and the func_802375A8 arg, but
    // tests the loop bound with a bare cmplwi - a u8 counter would force an
    // extra rlwinm mask before the bound test). n declared first: saved-reg
    // allocation gives it the lower register (retail n->r25, i->r26).
    unsigned int n = 2;
    unsigned int i = 1;
    do {
        sprintf(buf, lbl_eu_8050B00C + 0x1b2, (u8)i);
        u8 f = func_801361E8(self->field_0x4C, buf, self->field_0x55);
        if (f != 0) {
            func_802375A8(self, f, (u8)n++);
        }
        i++;
    } while (i <= 0xA);
}
#pragma optimize_for_size off

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

// func_80236DB8 - fetch stat block via vtable[0x224] dispatch, return s16 id.
// Declared int (not s16): the callee already sign-extends the s16 slot value
// in r3, so callers (func_80238038/802384F4) add it directly without an
// explicit extsh (retail codegen).
int func_80236DB8(CArtsInfo* self) {
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
// noinline + extern "C": the arts-level callers (func_8023939C) must emit a
// bl to the retail (unmangled) symbol, not an inlined vtable dispatch.
extern "C" __declspec(noinline) int func_80236E28(CArtsInfo* self) {
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
    int weapon = obj->field_0x26;
    if (weapon == -1) goto fail;
    CArtsInfoListEntry* e = func_80157C4C(2);
    if (e == 0) goto fail;
    u32 v0 = e->field_0x0;
    if (v0 == 0) goto fail;
    u16 id = func_80139358(v0 >> 20);
    int hp = (int)func_80136254((const void*)lbl_eu_806640F4, lbl_eu_8050B00C + 0x1f8, id);
    u8 b = (u8)func_801361E8(lbl_eu_806640F4, lbl_eu_8050B00C + 0x200, id);
    if ((b & 4) != 0) {
        int base = func_800A082C(obj);
        // Both operands are masked to u16 before the multiply; the scaled
        // product goes through the shared s32->f64 magic and is clamped to
        // 999 with an unsigned compare.
        hp = (int)(lbl_eu_80668690 * ConvS32ToF64((hp & 0xffff) * (base & 0xffff)));
        if ((hp & 0xffffu) >= 0x3e7u) hp = 0x3e7;  // unsigned cmpli, clamp 999
    }
    // Both lookups inline: MWCC evaluates + right-to-left, so func_802370A8
    // (rightmost) runs first and its result survives in r31 like retail.
    return (int)(lbl_eu_80668694 * ConvS32ToF64((hp & 0xffff) * ((int)func_80236E6C(self, 0x52) + (int)func_802370A8(self) + 100))) & 0xffff;
fail:
    return 0;
}

// func_80237394 - arts info damage/level helper. Same shape as
// func_80237238 but the name-row string offset is 0x205 and the HP
// scale factor is lbl_eu_806686A0.
int func_80237394(CArtsInfo* self) {
    CArtsCharData* obj = (CArtsCharData*)func_8009EC9C(self->field_0x54);
    if (obj->field_0x26 == -1) goto fail;
    CArtsInfoListEntry* e = func_80157C4C(2);
    if (e == 0) goto fail;
    u32 v0 = e->field_0x0;
    if (v0 == 0) goto fail;
    u16 id = func_80139358(v0 >> 20);
    int hp = (int)func_80136254((const void*)lbl_eu_806640F4, lbl_eu_8050B00C + 0x205, id);
    u8 b = (u8)func_801361E8(lbl_eu_806640F4, lbl_eu_8050B00C + 0x200, id);
    if ((b & 4) != 0) {
        // u16 local: retail masks the scale at definition (clrlwi in r3).
        u16 base = (u16)func_800A082C(obj);
        hp = (int)(lbl_eu_806686A0 * (float)((hp & 0xffff) * base));
        if ((hp & 0xffffu) >= 0x3e7u) hp = 0x3e7;  // unsigned cmpli, clamp 999
    }
    // Both lookups inline: MWCC evaluates + right-to-left, so func_802370A8
    // (rightmost) runs first and its result survives in r31 like retail.
    return (int)(lbl_eu_80668694 * (float)((hp & 0xffff) * ((int)func_80236E6C(self, 0x53) + (int)func_802370A8(self) + 100))) & 0xffff;
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

// func_802375A8 - arts list entry update. Formats the current arts-row name
// (func_8013639C row lookup for field_0x50 at table 0x1ab) and the level
// string (arg3 + 2) into a buffer bound to the row pane on both layouts
// (layout 2 only when field_0x56 < 0xA), then dispatches on the row id
// (func_801361E8 lookup at 0x225, masked to u8) through a 44-case jump table
// to the per-arts text-update handlers.
//
// Dispatch-target forward declarations: C linkage so the jump-table calls
// emit the bare retail reloc names, and noinline so -inline auto keeps the
// bl's (the definitions below inherit the extern "C" linkage).
extern "C" __declspec(noinline) void func_80237A0C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80237B88(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80237D58(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80237E24(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80238038(CArtsInfo*, u32, int, u8);
extern "C" __declspec(noinline) void func_80238298(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_802384F4(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80238904(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80239030(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023916C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023939C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023959C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_802397F4(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80239964(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80239AA0(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80239BDC(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80239D20(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80239EFC(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80239FC4(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A148(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A210(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A2D8(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A398(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A460(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A55C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A60C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A6BC(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A76C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A81C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A8CC(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A97C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023AA2C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023AADC(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023AB8C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023AD5C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023AE24(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023AF60(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023B074(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023B12C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023B280(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023B368(CArtsInfo*, u32, int);

extern "C" __declspec(noinline) void func_802375A8(CArtsInfo* self, u8 arg2, u8 arg3) {
    char buf[32]; // sprintf at +0x8
    char* s = func_8013639C((const void*)self->field_0x50, lbl_eu_8050B00C + 0x1ab, arg2);
    sprintf(buf, lbl_eu_8050B00C + 0x218, arg3 + 2);
    func_80136B4C(self->mpLayout1, buf, s, 0);
    if (self->field_0x56 < 0xA) {
        func_80136B4C(self->mpLayout2, buf, s, 0);
    }
    u8 k = (u8)func_801361E8(self->field_0x50, lbl_eu_8050B00C + 0x225, arg2);
    switch (k) {
    case 0x00: func_80237A0C(self, arg2, arg3); break;
    case 0x01: func_80237B88(self, arg2, arg3); break;
    case 0x02: func_80237D58(self, arg2, arg3); break;
    case 0x03: func_80237E24(self, arg2, arg3); break;
    case 0x04: func_80238038(self, arg2, arg3, 0); break;
    case 0x05: func_80238298(self, arg2, arg3); break;
    case 0x06: func_802384F4(self, arg2, arg3); break;
    case 0x07: func_80238904(self, arg2, arg3); break;
    case 0x08: func_80239030(self, arg2, arg3); break;
    case 0x09: func_8023916C(self, arg2, arg3); break;
    case 0x0A: func_8023939C(self, arg2, arg3); break;
    case 0x0B: func_8023959C(self, arg2, arg3); break;
    case 0x0C: func_802397F4(self, arg2, arg3); break;
    case 0x0D: func_80239964(self, arg2, arg3); break;
    case 0x0E: func_80239AA0(self, arg2, arg3); break;
    case 0x0F: func_80239BDC(self, arg2, arg3); break;
    case 0x10: func_80239D20(self, arg2, arg3); break;
    case 0x11: func_80239EFC(self, arg2, arg3); break;
    case 0x12: func_80239FC4(self, arg2, arg3); break;
    case 0x13: func_8023A148(self, arg2, arg3); break;
    case 0x14: func_8023A210(self, arg2, arg3); break;
    case 0x15: func_8023A2D8(self, arg2, arg3); break;
    case 0x16: func_8023A398(self, arg2, arg3); break;
    case 0x17: func_8023A460(self, arg2, arg3); break;
    case 0x18: func_8023A55C(self, arg2, arg3); break;
    case 0x19: func_8023A60C(self, arg2, arg3); break;
    case 0x1A: func_8023A6BC(self, arg2, arg3); break;
    case 0x1B: func_8023A76C(self, arg2, arg3); break;
    case 0x1C: func_8023A81C(self, arg2, arg3); break;
    case 0x1D: func_8023A8CC(self, arg2, arg3); break;
    case 0x1E: func_8023A97C(self, arg2, arg3); break;
    case 0x1F: func_8023AA2C(self, arg2, arg3); break;
    case 0x20: func_8023AADC(self, arg2, arg3); break;
    case 0x21: func_8023AB8C(self, arg2, arg3); break;
    case 0x22: func_8023AD5C(self, arg2, arg3); break;
    case 0x23: func_8023AE24(self, arg2, arg3); break;
    case 0x24: func_8023AF60(self, arg2, arg3); break;
    case 0x25: func_8023B074(self, arg2, arg3); break;
    case 0x26: func_8023B12C(self, arg2, arg3); break;
    case 0x27: func_8023B280(self, arg2, arg3); break;
    case 0x28: func_80238038(self, arg2, arg3, (u8)(self->field_0x55 + 1)); break;
    case 0x29: func_80238038(self, arg2, arg3, (u8)(self->field_0x55 + 2)); break;
    case 0x2A: func_80238038(self, arg2, arg3, (u8)(self->field_0x55 + 3)); break;
    case 0x2B: break;
    case 0x2C: func_8023B368(self, arg2, arg3); break;
    default: break;
    }
}

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

// func_80237B88 - arts info text update. Reads the character-data category
// (func_800A32BC), discards a 0x24c-keyed lookup, then formats
// v - 0.5 * (level-1) * v * w (float arithmetic via MWCC's u32/s32
// conversion magics: clrlwi + 2^52 for the u8 lookups, xoris + 2^52+2^31
// for the signed (level-1)) into buf1, post-processes it, pushes it onto
// layout 1, then on level-up (< 0xA) formats the next-level value
// (level instead of level-1) into buf3 for layout 2 and runs the
// colour-pair helper when the raw lookup v differs from the next value.
void func_80237B88(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x48
    char buf2[32]; // sprintf at +0x28
    char buf3[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    func_800A32BC(func_8009EC9C(self->field_0x54));
    func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x24c, self->field_0x55);
    u8 v = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x250, self->field_0x55);
    u8 w = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x257, self->field_0x55);
    // fv/fw stay in saved FPRs across the calls (reused in the level-up
    // branch below); retail keeps them in f31/f30.
    float fv = (float)v;
    float fw = (float)w;
    float cur = fv - lbl_eu_80668694 * ((float)(self->field_0x56 - 1) * (fv * fw));
    sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, cur, s2);
    func_eu_80136F90(buf1);
    func_eu_8023D490(self, arg2, buf1);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        // (int) cast: retail converts level with the s32 magic (xoris);
        // plain (float)u8 would use the shorter u32 magic.
        float nxt = fv - lbl_eu_80668694 * ((float)(int)self->field_0x56 * (fv * fw));
        sprintf(buf3, lbl_eu_8050B00C + 0x232, s1, nxt, s2);
        func_eu_80136F90(buf3);
        func_eu_8023D490(self, arg2, buf3);
        func_80136A1C(self->mpLayout2, buf2, buf3, 0);
        if (fv != nxt) {
            func_80139A18(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_80237D58 - arts info text update. Same shape as func_8023A148 but the
// lookup is func_801360CC (s8 result): the value is abs()'d and narrowed back
// to s8 (retail: extsb after the abs call) for the sprintf vararg.
// Entry param shadows: declaring arg2's shadow first asks MWCC to copy r4
// before r3 in the entry block (retail order).
// u32 const on arg2: prologue-scheduling lever (KB: const params altered
// entry-store order in matched siblings); retail copies r4 before r3.
// Residual 2-instr reg swap: entry param-copy emission order (retail births
// r4->r29 before r3->r27). Documented MWCC claim-order invariant - not
// source-controllable (see func_8023916C / func_8023AADC notes).
void func_80237D58(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    s8 r = func_801360CC(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x263, self->field_0x55);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, (s8)abs(r), s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_80237E24 - arts info text update. The first grid value (u16-keyed
// lookup func_8013606C at 0x26d) is converted to float and scaled by
// lbl_eu_806686A4 when non-zero, then overridden by the field_0x55
// arts-type switch (0x68/0x69/0x70 -> fixed constants); the second grid
// value (u8-keyed lookup func_8013600C at 0x275) is scaled the same way.
// The displayed value is base + second*(level-1), post-processed and pushed
// onto both layouts; on level-up the level grid is recomputed for layout 2
// and the colour-pair helper runs when the two values differ.
// Entry param-save-order wall (see func_8023916C).
void func_80237E24(CArtsInfo* self_, u32 arg2_, int arg3_) {
    u32 arg2 = arg2_;
    CArtsInfo* self = self_;
    int arg3 = arg3_;
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    u16 v1 = func_8013606C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x26d, self->field_0x55);
    float f1 = (float)v1;
    u8 v2 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x275, self->field_0x55);
    float f2 = (float)v2;
    if (f1 != lbl_eu_80668680) {
        f1 = f1 * lbl_eu_806686A4;
    }
    switch (self->field_0x55) {
    case 0x68:
        f1 = lbl_eu_806686B0;
        break;
    case 0x69:
        f1 = lbl_eu_806686B4;
        break;
    case 0x70:
        f1 = lbl_eu_806686B8;
        break;
    default:
        break;
    }
    if (f2 != lbl_eu_80668680) {
        f2 = f2 * lbl_eu_806686A4;
    }
    float cur = f1 + f2 * (float)(self->field_0x56 - 1);
    sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, cur, s2);
    func_eu_80136F90(buf1);
    func_eu_8023D490(self, arg2, buf1);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        float nxt = f1 + f2 * (float)(int)self->field_0x56;
        sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, nxt, s2);
        func_eu_80136F90(buf1);
        func_eu_8023D490(self, arg2, buf1);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            func_80139A18(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_80238038 - arts info text update. Same shape as func_80238298
// (stat + min(damage1, damage2) grids scaled by 0.5) but the grid lookups
// func_80237100/8023719C are keyed on the 4th parameter instead of 0 and
// the stat comes from func_80236DB8.
void func_80238038(CArtsInfo* self, u32 arg2, int arg3, u8 arg4) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    int level;
    func_802374F0(self, arg2);
    func_8023754C(self, arg2);
    s16 stat = func_80236DB8(self);
    int g1 = func_80237100(self, self->field_0x56, arg4);
    int g2 = func_8023719C(self, self->field_0x56, arg4);
    int m1 = func_80237238(self);
    int m2 = func_80237394(self);
    if (m1 > m2) m1 = m2;
    int s = m1 + stat;
    int v1 = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(g1 * s)));
    int t = m2 + stat;
    int v2 = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(g2 * t)));
    char* str = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
    sprintf(buf1, lbl_eu_8050B00C + 0x282, v1, str, v2);
    level = arg3 + 2;
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        int g1n = func_80237100(self, self->field_0x56 + 1, arg4);
        int g2n = func_8023719C(self, self->field_0x56 + 1, arg4);
        int v1n = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(g1n * s)));
        int v2n = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(g2n * t)));
        char* str2 = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
        sprintf(buf1, lbl_eu_8050B00C + 0x282, v1n, str2, v2n);
        sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        func_80139A18(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
    }
}

// func_80238298 - arts info text update. Combines the stat id
// (func_80236DF0) with the min of the two damage/level helpers
// (func_80237238/80237394): the two grids are 0.5*stat*(m+stat) products,
// formatted into buf1 (sprintf 0x282). On level-up the level+1 grids are
// recomputed for layout 2 and the colour-pair helper always runs.
void func_80238298(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    int level;
    func_802374F0(self, arg2);
    func_8023754C(self, arg2);
    int stat = func_80236DF0(self);
    int g1 = func_80237100(self, self->field_0x56, 0);
    int g2 = func_8023719C(self, self->field_0x56, 0);
    int m1 = func_80237238(self);
    int m2 = func_80237394(self);
    if (m1 > m2) m1 = m2;
    int s = m1 + stat;
    int v1 = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(g1 * s)));
    int t = m2 + stat;
    int v2 = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(g2 * t)));
    char* str = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
    sprintf(buf1, lbl_eu_8050B00C + 0x282, v1, str, v2);
    level = arg3 + 2;
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        int g1n = func_80237100(self, self->field_0x56 + 1, 0);
        int g2n = func_8023719C(self, self->field_0x56 + 1, 0);
        int v1n = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(g1n * s)));
        int v2n = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(g2n * t)));
        char* str2 = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
        sprintf(buf1, lbl_eu_8050B00C + 0x282, v1n, str2, v2n);
        sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        func_80139A18(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
    }
}

// func_802384F4 - arts info text update. Combines the stat id
// (func_80236DB8) with the min of the two damage/level helpers
// (func_80237238/80237394): the two grids are 0.5*stat*(m+stat) products
// (func_801C6158), then both grids are scaled by a byte-keyed factor
// (func_8013600C at 0x289) and get the 0x49-keyed arts lookup
// (func_80236E6C) contribution added: value = v + 0.5*v*func_80236E6C(0x49).
// Two func_8013606C lookups (0x26d/0x290) are called with results discarded.
void func_802384F4(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    int level;
    func_802374F0(self, arg2);
    func_8023754C(self, arg2);
    int stat = func_80236DB8(self);
    int g1 = func_80237100(self, self->field_0x56, 0);
    int g2 = func_8023719C(self, self->field_0x56, 0);
    int m1 = func_80237238(self);
    int m2 = func_80237394(self);
    if (m1 > m2) m1 = m2;
    int s = m1 + stat;
    int v1 = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(g1 * s)));
    int t = m2 + stat;
    int v2 = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(g2 * t)));
    u8 v3 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    func_8013606C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x26d, self->field_0x55);
    func_8013606C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x290, self->field_0x55);
    v1 = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v1 * v3)));
    int a = func_80236E6C(self, 0x49);
    int value1 = v1 + func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v1 * a)));
    v2 = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v2 * v3)));
    a = func_80236E6C(self, 0x49);
    int value2 = v2 + func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v2 * a)));
    char* str = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
    sprintf(buf1, lbl_eu_8050B00C + 0x282, value1, str, value2);
    level = arg3 + 2;
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        int g1n = func_80237100(self, self->field_0x56 + 1, 0);
        int g2n = func_8023719C(self, self->field_0x56 + 1, 0);
        int v1n = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(g1n * s)));
        int v2n = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(g2n * t)));
        v1n = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v1n * v3)));
        a = func_80236E6C(self, 0x49);
        int value1n = v1n + func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v1n * a)));
        v2n = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v2n * v3)));
        a = func_80236E6C(self, 0x49);
        int value2n = v2n + func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v2n * a)));
        char* str2 = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
        sprintf(buf1, lbl_eu_8050B00C + 0x282, value1n, str2, value2n);
        sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        func_80139A18(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
    }
}

// func_80238904 - arts info text update. Same stat+min(damage1,damage2)
// grid shape as func_80238298 (func_80236DF0 stat, func_80237100/8023719C
// offsets), then the grids are scaled by a field_0x55 arts-type base
// (func_8013600C at 0x289, overridden by the 0x68/0x69/0x70 switch to fixed
// 0x28/0x3c/0x64). A func_8013606C lookup at 0x297 picks the arts lookup
// key (func_80236E6C) used to add 0.5*v*lookup to each grid: 0x66 -> 0x49,
// 0x67 -> 0x9, 0x68 -> 0x3a, 0x69 -> 0x5c. On level-up the level+1 grids
// are recomputed for layout 2 (re-dispatching the 0x297 key switch) and the
// colour-pair helper always runs.
void func_80238904(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    int level;
    func_802374F0(self, arg2);
    func_8023754C(self, arg2);
    int stat = func_80236DF0(self);
    int g1 = func_80237100(self, self->field_0x56, 0);
    int g2 = func_8023719C(self, self->field_0x56, 0);
    int m1 = func_80237238(self);
    int m2 = func_80237394(self);
    if (m1 > m2) m1 = m2;
    int s = m1 + stat;
    int v1 = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(g1 * s)));
    int t = m2 + stat;
    int v2 = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(g2 * t)));
    int base = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    switch (self->field_0x55) {
    case 0x68:
        base = 0x28;
        break;
    case 0x69:
        base = 0x3c;
        break;
    case 0x70:
        base = 0x64;
        break;
    default:
        break;
    }
    v1 = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v1 * base)));
    v2 = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v2 * base)));
    u16 k = func_8013606C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x297, self->field_0x55);
    if (k == 0x66) {
        int a = func_80236E6C(self, 0x49);
        v1 += func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v1 * a)));
        a = func_80236E6C(self, 0x49);
        v2 += func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v2 * a)));
    } else if (k == 0x67) {
        int a = func_80236E6C(self, 0x9);
        v1 += func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v1 * a)));
        a = func_80236E6C(self, 0x9);
        v2 += func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v2 * a)));
    } else if (k == 0x68) {
        int a = func_80236E6C(self, 0x3a);
        v1 += func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v1 * a)));
        a = func_80236E6C(self, 0x3a);
        v2 += func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v2 * a)));
    } else if (k == 0x69) {
        int a = func_80236E6C(self, 0x5c);
        v1 += func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v1 * a)));
        a = func_80236E6C(self, 0x5c);
        v2 += func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v2 * a)));
    }
    char* str = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
    sprintf(buf1, lbl_eu_8050B00C + 0x282, v1, str, v2);
    level = arg3 + 2;
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        int g1n = func_80237100(self, self->field_0x56 + 1, 0);
        int g2n = func_8023719C(self, self->field_0x56 + 1, 0);
        int v1n = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(g1n * s)));
        int v2n = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(g2n * t)));
        v1n = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v1n * base)));
        v2n = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v2n * base)));
        u16 k2 = func_8013606C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x297, self->field_0x55);
        if (k2 == 0x66) {
            int a = func_80236E6C(self, 0x49);
            v1n += func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v1n * a)));
            a = func_80236E6C(self, 0x49);
            v2n += func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v2n * a)));
        } else if (k2 == 0x67) {
            int a = func_80236E6C(self, 0x9);
            v1n += func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v1n * a)));
            a = func_80236E6C(self, 0x9);
            v2n += func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v2n * a)));
        } else if (k2 == 0x68) {
            int a = func_80236E6C(self, 0x3a);
            v1n += func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v1n * a)));
            a = func_80236E6C(self, 0x3a);
            v2n += func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v2n * a)));
        } else if (k2 == 0x69) {
            int a = func_80236E6C(self, 0x5c);
            v1n += func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v1n * a)));
            a = func_80236E6C(self, 0x5c);
            v2n += func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(v2n * a)));
        }
        char* str2 = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
        sprintf(buf1, lbl_eu_8050B00C + 0x282, v1n, str2, v2n);
        sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        func_80139A18(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
    }
}

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

// func_8023916C - arts grid percentage. Both string lookups are called but
// their results discarded; the stat id (func_80236DF0) is multiplied by two
// grid offsets (func_80237100/8023719C keyed on the arts level), each scaled
// by 0.5 and converted via func_801C6158, then formatted with the
// func_80136190(0x32, 0x3d, 0x52) string into buf1. On level-up (< 0xA) the
// level+1 offsets are recomputed and formatted on layout 2 and the
// colour-pair helper always runs.
// Entry param-save-order wall: retail copies arg2 (r4) before self (r3);
// MWCC invariantly emits the param-1 (r3) copy first (same ABI-boundary
// artifact as CMenuShopSell func_8018B130). Shadows kept for documentation.
void func_8023916C(CArtsInfo* self_, u32 arg2_, int arg3_) {
    u32 arg2 = arg2_;
    CArtsInfo* self = self_;
    int arg3 = arg3_;
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    // level assigned late (after sprintf1) so the addi lands after the first
    // sprintf like retail's in-place addi r27, r26, 2.
    int level;
    func_802374F0(self, arg2);
    func_8023754C(self, arg2);
    int stat = func_80236DF0(self);
    int g1 = func_80237100(self, self->field_0x56, 0);
    int g2 = func_8023719C(self, self->field_0x56, 0);
    int v1 = func_801C6158(lbl_eu_80668694 * (float)(g1 * stat));
    int v2 = func_801C6158(lbl_eu_80668694 * (float)(g2 * stat));
    char* str = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
    sprintf(buf1, lbl_eu_8050B00C + 0x282, v1, str, v2);
    level = arg3 + 2;
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        int g1n = func_80237100(self, self->field_0x56 + 1, 0);
        int g2n = func_8023719C(self, self->field_0x56 + 1, 0);
        int v1n = func_801C6158(lbl_eu_80668694 * (float)(g1n * stat));
        int v2n = func_801C6158(lbl_eu_80668694 * (float)(g2n * stat));
        char* str2 = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
        sprintf(buf1, lbl_eu_8050B00C + 0x282, v1n, str2, v2n);
        sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        func_80139A18(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
    }
}

// func_8023939C - arts info text update. The stat value (func_80236E28) is
// halved into cur, combined with a grid offset (v1 + v2*(level-1)) and halved
// again, then formatted into buf1. On level-up (< 0xA) the lookups are
// re-fetched and the level grid recomputed for layout 2; the colour-pair
// helper runs when the two formatted values differ.
void func_8023939C(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    int base = func_80236E28(self);
    u8 v1 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    u8 v2 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
    float fbase = (float)(base * 100);
    int cur = (int)(lbl_eu_80668694 * fbase);
    u32 grid = v1 + v2 * (self->field_0x56 - 1);
    float fcur = (float)(cur * grid);
    int cur2 = (int)(lbl_eu_80668694 * fcur);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, cur2, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u8 n1 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
        u8 n2 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
        u8 lvl = self->field_0x56;
        u32 grid2 = n1 + n2 * lvl;
        float fnxt = (float)(cur * grid2);
        int nxt = (int)(lbl_eu_80668694 * fnxt);
        sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, nxt, s2);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur2 != nxt) {
            func_80139A18(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_8023959C - arts info text update. The stat value (func_80236E28,
// (int)float at +0x10) is scaled by 100 then halved into x1; the two grids
// are x1*g1 and x1*g2, each halved, formatted into buf1 (sprintf 0x282). On
// level-up the level+1 grids are recomputed for layout 2 and the colour-pair
// helper always runs.
void func_8023959C(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    int level;
    func_802374F0(self, arg2);
    func_8023754C(self, arg2);
    int stat = func_80236E28(self);
    int g1 = func_80237100(self, self->field_0x56, 0);
    int g2 = func_8023719C(self, self->field_0x56, 0);
    int x1 = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(stat * 100)));
    int x2 = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(x1 * g1)));
    int x3 = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(x1 * g2)));
    char* str = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
    sprintf(buf1, lbl_eu_8050B00C + 0x282, x2, str, x3);
    level = arg3 + 2;
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        int g1n = func_80237100(self, self->field_0x56 + 1, 0);
        int g2n = func_8023719C(self, self->field_0x56 + 1, 0);
        int x2n = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(x1 * g1n)));
        int x3n = func_801C6158((float)(lbl_eu_80668694 * ConvS32ToF64(x1 * g2n)));
        char* str2 = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
        sprintf(buf1, lbl_eu_8050B00C + 0x282, x2n, str2, x3n);
        sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        func_80139A18(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
    }
}

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
    u32 grid1;
    u32 grid2;
    func_802374F0(self, arg2);
    func_8023754C(self, arg2);
    grid1 = func_80237100(self, self->field_0x56, 0);
    grid2 = func_8023719C(self, self->field_0x56, 0);
    char* str = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
    sprintf(buf1, lbl_eu_8050B00C + 0x282, grid1, str, grid2);
    // Eager intermediate forces the addi next to sprintf1 like retail
    // (addi r30, r30, 2).
    level = arg3 + 2;
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        grid2 = func_80237100(self, self->field_0x56 + 1, 0);
        grid1 = func_8023719C(self, self->field_0x56 + 1, 0);
        char* str2 = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
        sprintf(buf1, lbl_eu_8050B00C + 0x282, grid2, str2, grid1);
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

// func_80239D20 - arts info text update. The two byte lookups are converted
// to float; when the first is non-zero it is scaled by lbl_eu_806686A4, then
// the value scale*v2*(level-1) is added and formatted into buf1. On level-up
// (< 0xA) the level grid is recomputed for layout 2; the colour-pair helper
// runs when the two values differ.
// Entry param-save-order wall (see func_8023916C).
// fb declared (uninitialized) before fa: MWCC colours equal-lifetime FPRs
// in reverse declaration order, and retail keeps fb in f31 / fa in f30.
void func_80239D20(CArtsInfo* self_, u32 arg2_, int arg3_) {
    u32 arg2 = arg2_;
    CArtsInfo* self = self_;
    int arg3 = arg3_;
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    // fb declared (uninitialized) before fa: MWCC colours equal-lifetime FPRs
    // in reverse declaration order, and retail keeps fb in f31 / fa in f30.
    // The conversion statements keep the retail evaluation order.
    float fb;
    u8 v1 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    float fa = (float)v1;
    u8 v2 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
    fb = (float)v2;
    if (fa != lbl_eu_80668680) {
        fa = fa * lbl_eu_806686A4;
    }
    float cur = fa + lbl_eu_806686A4 * (fb * (float)(self->field_0x56 - 1));
    sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, cur, s2);
    func_eu_80136F90(buf1);
    func_eu_8023D490(self, arg2, buf1);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        float nxt = fa + lbl_eu_806686A4 * (fb * (float)(int)self->field_0x56);
        sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, nxt, s2);
        func_eu_80136F90(buf1);
        func_eu_8023D490(self, arg2, buf1);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            func_80139A18(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_8023AB8C - arts info text update. Formats
// 1.0 + scale*(float)v1 + (float)v2*(level-1) into buf1, post-processes it
// and pushes it onto layout 1. On level-up (< 0xA) the level grid is
// recomputed (level instead of level-1, s32 magic) for layout 2; the
// colour-pair helper runs when the two values differ.
// Entry param-save-order wall (see func_8023916C).
void func_8023AB8C(CArtsInfo* self_, u32 arg2_, int arg3_) {
    u32 arg2 = arg2_;
    CArtsInfo* self = self_;
    int arg3 = arg3_;
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
    u8 v1 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    float fa = (float)v1;
    u8 v2 = func_8013600C(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
    float fb = (float)v2;
    float cur = lbl_eu_80668684 + (lbl_eu_806686A4 * fa + fb * (float)(self->field_0x56 - 1));
    sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, cur, s2);
    func_eu_80136F90(buf1);
    func_eu_8023D490(self, arg2, buf1);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        float nxt = lbl_eu_80668684 + (lbl_eu_806686A4 * fa + fb * (float)(int)self->field_0x56);
        sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, nxt, s2);
        func_eu_80136F90(buf1);
        func_eu_8023D490(self, arg2, buf1);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            func_80139A18(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}
// func_80239EFC - arts info text update. Formats the arts skill name and a
// byte-keyed table value (func_8013600C) into buf1, the level string into
// buf2, then pushes both onto the layouts (layout 2 only when field_0x56
// < 0xA). Same shape as func_8023A55C but the 3rd sprintf arg is a lookup.
void func_80239EFC(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
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
// NOTE: retail lfds the 2^52 correction constants from the shared sdata2
// labels (lbl_eu_806686A8 unsigned / lbl_eu_80668698 signed); MWCC pools the
// plain-cast equivalents as TU-local @N labels - needs a CArtsInfo.o
// pool_patterns postprocess rule (values 43300000_00000000 / 43300000_80000000).
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
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
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
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
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
    u32 a2 = arg2;
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
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
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
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
#pragma optimize_for_size on
// Shadowed locals keep the retail register mapping (arg2->r31, self->r28);
// the residual 2-instruction reg swap is the entry param-save order
// (retail copies r4->r31 before r3->r28; see func_8023916C).
void func_8023A55C(CArtsInfo* self_, u32 arg2_, int arg3_) {
    u32 arg2 = arg2_;
    int arg3 = arg3_;
    CArtsInfo* self = self_;
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, 5, func_8023754C(self, arg2));
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
// func_8023A60C - arts info text update. Formats the skill name, a double
// constant and the second skill string into buf1, post-processes it
// (func_eu_80136F90) and hands it to func_eu_8023D490 before the usual
// level-string + dual-layout push (layout 2 only when field_0x56 < 0xA).
// Residual 2-instruction reg swap is the retail entry param-save order:
// retail copies r4 before r3; MWCC invariantly emits the param-1 (r3) copy
// first (same ABI-boundary artifact as CMenuShopSell func_8018B130).
void func_8023A60C(CArtsInfo* self_, u32 arg2_, int arg3_) {
    // Shadow params in retail's save order (arg2 copy before self copy).
    u32 arg2 = arg2_;
    CArtsInfo* self = self_;
    int arg3 = arg3_;
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
#pragma optimize_for_size off

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
// Entry param-save-order wall (see func_8023916C): retail saves arg2 (r31)
// before self (r28); MWCC invariantly emits the param-1 copy first.
#pragma optimize_for_size on
void func_8023AA2C(CArtsInfo* self_, u32 arg2_, int arg3_) {
    u32 arg2 = arg2_;
    CArtsInfo* self = self_;
    int arg3 = arg3_;
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
// Residual: retail spills arg2->r31 BEFORE self->r28; MWCC (size-mode, which
// is required for the retail stmw r28 frame) always copies r3 first. This is
// the documented param-save claim-order invariant (MWCC_CASES "VM-op/
// saved-param claim order", OPEN) - not source-controllable.
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
// Entry param-save-order artifact (unit-wide, ~40 dispatch handlers): retail
// copies arg2 (r4->r25) BEFORE self (r3->r31) at entry; our build invariantly
// emits the param-1 copy first. Shadow-locals, early local copy of arg2, and
// buf declaration reordering were all tried - none change the copy order
// (copy-locals coalesce; decl reorder swaps frame offsets; optimize_for_size
// is neutral here - unlike CTagProcessor func_801289B4). Same 2-op
// reg_swap present in func_80239030/39AA0/916C/9EFC/A55C.
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
    char* s1 = func_802374F0(self, arg2);
    char* s2 = func_8023754C(self, arg2);
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
void func_8023B074(CArtsInfo* self_, u32 arg2_, int arg3_) {
    // Shadow params in retail's save order (arg2 before self before arg3).
    u32 arg2 = arg2_;
    CArtsInfo* self = self_;
    int arg3 = arg3_;
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
void func_8023B12C(CArtsInfo* self_, u32 arg2_, int arg3_) {
    // Shadow params in retail's save order (arg2 before self before arg3).
    u32 arg2 = arg2_;
    func_802374F0(self_, arg2);
    CArtsInfo* self = self_;
    func_8023754C(self, arg2);
    int arg3 = arg3_;
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
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

int CArtsInfo::OnFileEvent(CEventFile* event) { return 0; }



// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Static constructor: initialise the arts font/colour small-data entries
// (same shape as CItemBoxInfo's sinit_801EABC4).
void sinit_8023BC8C() {
    func_801D1F9C(&lbl_eu_80664748, 0);
    func_801D1F9C(&lbl_eu_80664750, 0);
    func_801C4B60(&lbl_eu_80664758, 0xff, 0xff, 0xfa, 0);
    func_801C4B60(&lbl_eu_80664760, 0x25, 0x8a, 0xce, 0);
}
