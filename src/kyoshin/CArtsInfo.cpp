// Auto-scaffolded catalog TU for kyoshin/CArtsInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CArtsInfo.hpp"
#include <nw4r/lyt.h>

// Extern declarations for referenced symbols

// String table base (rodata, accessed via sda21 relocation)
extern char lbl_eu_8050B00C[];

// Vtable symbol for CArtsInfo
extern void* lbl_eu_80536A88;

// Jump table for state machine dispatch (func_8023587C)
extern void* jumptable_eu_805369A0[];

// Float constants in small data area
extern float lbl_eu_80668680; // 0.0f
extern float lbl_eu_80668684; // 1.0f

// Small data string pairs for func_802369C0
extern char lbl_eu_80664748[];
extern char lbl_eu_80664750[];

// External function declarations
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" u32 func_800A9D90();
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void func_8003AA8C__5CBdatFUl(u32);
extern "C" void func_801390E0__FPP11CFileHandle(void*);
extern "C" void func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(void*);
extern "C" void func_8045F778__17UnkClass_8045F564Fv(void*);
extern "C" UnkClass_8045F564* __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __ct__CCur18(void*, void*);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dl__FPv(void*);
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
extern "C" u32 func_80137444__FPQ34nw4r3lyt13AnimTransformf(nw4r::lyt::AnimTransform*, float);
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);
extern "C" void func_80136B4C(void*, const char*, void*, u32);
extern "C" void func_80136A1C(void*, const char*, void*, u32);
extern "C" void* func_80136190(const char*, const char*, int);
extern "C" void func_80138078__FUl(u32);
extern "C" u32 func_8013639C(u32, const char*, u32);
extern "C" u32 func_801361E8(u32, const char*, u32);
extern "C" u32 func_80139A18(void*, const char*, void*, void*);
extern "C" u32 func_80137924(void*, void*, void*, void*);
extern "C" u32 func_8009EC9C(u32);
extern "C" void func_801D202C(void*);
extern "C" void func_801D20B0(void*, void*);
extern "C" void func_801D216C(void*, int);

// sprintf is provided by the C runtime
extern "C" int sprintf(char*, const char*, ...);

// Forward declarations for state machine functions (defined later in this TU)
void func_80236508(CArtsInfo*);
void func_802369C0(CArtsInfo*);
void func_80236CF4(CArtsInfo*);

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
    typedef void (*VirtFn)(void*, u32);
    VirtFn fn = ((VirtFn*)*(u32*)layout)[14]; // 0x38 / 4 = 14
    fn(layout, 0);
}

static inline void callVirt_2C_50(nw4r::lyt::Layout* layout, void* arg1, u32 arg2) {
    typedef void (*VirtFn)(void*, void*, u32);
    VirtFn fn = ((VirtFn*)*(u32*)layout)[11]; // 0x2C / 4 = 11
    fn(layout, arg1, arg2);
}

// Vtable virtual method at offset 0x08 with flags=1 (destructor)
static inline void callVirtDelete_08(void* obj) {
    typedef void (*VirtFn)(void*, int);
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
    typedef u32 (*VirtFn)(void*, const char*, u32);
    VirtFn fn = ((VirtFn*)*(u32*)layout)[15]; // 0x3C / 4 = 15
    return fn(layout, arg1, arg2);
}

u8 CArtsInfo::getField48() { return field_0x48; }

u8 CArtsInfo::getField49() { return field_0x49; }

// __ct__CArtsInfo — constructor
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

// __dt__9CArtsInfoFv — destructor
// .text:0xAC, size 0x60
CArtsInfo::~CArtsInfo() {
    // Destroy CCur18 cursor
    __dt__6CCur18Fv(mCursor, -1);

    // Destroy UnkClass_8045F564 member
    __dt__17UnkClass_8045F564Fv(&mMemRegion, -1);
}

// func_80235814 — file loading
// .text:0x10C, size 0x68
void func_80235814(CArtsInfo* self) {
    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    self->field_0x14 = (int)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)lbl_eu_8050B00C, (const char*)self, (void*)handle, 0, 0);

    func_800A9D90();
    self->field_0x18 = (int)readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)(lbl_eu_8050B00C + 0x15), (const char*)self, (void*)handle, 0, 0);
}

// func_8023587C — state machine dispatch
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

// func_80235958 — draw
// .text:0x250, size 0x74
void func_80235958(CArtsInfo* self, void* drawInfo) {
    if (self->field_0x40 == 0) return;

    func_80137038(self->mpLayout1, reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
    func_80137038(self->mpLayout2, reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);

    s8 cursorActive = self->field_0x5A;
    if (cursorActive >= 0) {
        func_801D20B0(self->mCursor, drawInfo);
    }
}

// func_802359CC — cleanup
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

// func_80235A90 — get field_0x48
// .text:0x388, size 0x8
u8 func_80235A90(CArtsInfo* self) { return self->field_0x48; }

// func_80235A98 — get field_0x49
// .text:0x390, size 0x8
u8 func_80235A98(CArtsInfo* self) { return self->field_0x49; }

// func_80235AA0 — state check (state==0 -> 1)
// .text:0x398, size 0x20
void func_80235AA0(CArtsInfo* self) {
    if (self->field_0x44 != 0) return;
    self->field_0x44 = 1;
    self->field_0x49 = 0;
}

// func_80235AC0 — state check (state==3 -> 4)
// .text:0x3B8, size 0x20
void func_80235AC0(CArtsInfo* self) {
    if (self->field_0x44 != 3) return;
    self->field_0x44 = 4;
    self->field_0x49 = 0;
}

// func_80235AE0 — large state machine (state==3 -> 6, layout animation setup)
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
    u32 artsData = func_8009EC9C(self->field_0x54);
    void* str1 = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3D, 0x18);

    char buf[64];
    sprintf(buf, lbl_eu_8050B00C + 0x42, *(u32*)((u8*)artsData + 0x17C), str1);

    func_80136A1C(self->mpLayout1, lbl_eu_8050B00C + 0x47, buf, 0);

    func_80138078__FUl(0x6D);
}

// func_80235D24 — state machine (state==9 -> 0xA)
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

// func_80235DD8 — state machine (state==9 -> 0xC)
// .text:0x6D0, size 0xAC
void func_80235DD8(CArtsInfo* self) {
    if (self->field_0x44 != 9) return;

    self->field_0x44 = 0xC;
    self->field_0x49 = 0;

    // Set cursor visibility
    func_801D216C(self->mCursor, 0);

    // Bind animations to layout 2
    callVirt_2C_50(self->mpLayout2, self->mpAnimTrans5, 0);
    callVirt_2C_50(self->mpLayout2, self->mpAnimTrans6, 1);

    // Set anim transform frame
    *(float*)((u8*)self->mpAnimTrans6 + 0x10) = lbl_eu_80668680;

    callVirt_38_0(self->mpLayout2);
}

// func_80235E84 — set field_0x54
// .text:0x77C, size 0x8
void func_80235E84(CArtsInfo* self, u8 val) { self->field_0x54 = val; }

// func_80235E8C — set field_0x55
// .text:0x784, size 0x8
void func_80235E8C(CArtsInfo* self, u8 val) { self->field_0x55 = val; }

// func_80235E94 — set field_0x56
// .text:0x78C, size 0x8
void func_80235E94(CArtsInfo* self, u8 val) { self->field_0x56 = val; }

// func_80235E9C — set field_0x58
// .text:0x794, size 0x8
void func_80235E9C(CArtsInfo* self, u16 val) { self->field_0x58 = val; }

// func_80235EA4 — call func_80236508, check fields, call func_802369C0
// .text:0x79C, size 0x4C
void func_80235EA4(CArtsInfo* self) {
    func_80236508(self);

    if (self->field_0x54 != 0 && self->field_0x55 != 0) {
        func_802369C0(self);
    }
}

// func_80235EF0 — decrement field_0x5A, tail-call func_80236CF4
// .text:0x7E8, size 0x24
void func_80235EF0(CArtsInfo* self) {
    s8 val = self->field_0x5A;
    val--;
    if (val < 0) val = 1;
    self->field_0x5A = val;
    func_80236CF4(self);
}

// func_80235F14 — increment field_0x5A, tail-call func_80236CF4
// .text:0x80C, size 0x28
void func_80235F14(CArtsInfo* self) {
    s8 val = self->field_0x5A;
    val++;
    if (val > 1) val = 0;
    self->field_0x5A = val;
    func_80236CF4(self);
}

// func_80235F3C — check if field_0x5A is zero
// .text:0x834, size 0x14
u32 func_80235F3C(CArtsInfo* self) {
    s8 val = self->field_0x5A;
    return (val == 0) ? 1 : 0;
}

// func_80235F50 — check if field_0x44 < 6
// .text:0x848, size 0x1C
u32 func_80235F50(CArtsInfo* self) {
    int state = self->field_0x44;
    return (state < 6) ? 1 : 0;
}

// func_80235F6C — animation state 0
// .text:0x864, size 0xB4
void func_80235F6C(CArtsInfo* self) {
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(self->mpAnimTrans1, lbl_eu_80668684) == 0) return;

    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans3, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans4, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans1, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans2, 1);

    self->field_0x44 = 2;
}

// func_80236020 — animation state 1
// .text:0x918, size 0x4C
void func_80236020(CArtsInfo* self) {
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(self->mpAnimTrans2, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 3;
    self->field_0x49 = 1;
}

// func_8023606C — animation state 2
// .text:0x964, size 0xB4
void func_8023606C(CArtsInfo* self) {
    if (func_80137510(self->mpAnimTrans2, lbl_eu_80668684) == 0) return;

    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans2, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans3, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans4, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans1, 1);

    self->field_0x44 = 5;
}

// func_80236120 — animation state 3
// .text:0xA18, size 0x4C
void func_80236120(CArtsInfo* self) {
    if (func_80137510(self->mpAnimTrans1, lbl_eu_80668684) == 0) return;

    self->field_0x49 = 1;
    self->field_0x44 = 0;
}

// func_8023616C — animation state 4
// .text:0xA64, size 0xB4
void func_8023616C(CArtsInfo* self) {
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(self->mpAnimTrans3, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 7;

    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans2, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans3, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans1, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans4, 1);
}

// func_80236220 — animation state 5
// .text:0xB18, size 0xB4
void func_80236220(CArtsInfo* self) {
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(self->mpAnimTrans4, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 8;

    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans2, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans3, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans1, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans4, 1);
}

// func_802362D4 — animation state 6
// .text:0xBCC, size 0x60
void func_802362D4(CArtsInfo* self) {
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(self->mpAnimTrans5, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 9;
    self->field_0x49 = 1;
    func_801D216C(self->mCursor, 1);
    func_80236CF4(self);
}

// func_80236334 — animation state 7
// .text:0xC2C, size 0xD4
void func_80236334(CArtsInfo* self) {
    u32 done1 = func_80137510(self->mpAnimTrans4, lbl_eu_80668684);
    u32 done2 = func_80137510(self->mpAnimTrans5, lbl_eu_80668684);

    if (done1 == 0 || done2 == 0) return;

    self->field_0x44 = 0xB;

    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans2, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans1, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans4, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans3, 1);
}

// func_80236408 — animation state 8
// .text:0xD00, size 0x4C
void func_80236408(CArtsInfo* self) {
    if (func_80137510(self->mpAnimTrans3, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 3;
    self->field_0x49 = 1;
}

// func_80236454 — animation state 9
// .text:0xD4C, size 0xB4
void func_80236454(CArtsInfo* self) {
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(self->mpAnimTrans6, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 0xB;

    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans2, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans1, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans4, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans3, 1);
}

// func_80236508 — large layout setup (bind all pane animations)
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
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0xEE, lbl_eu_80664748, lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0xFD, lbl_eu_80664748, lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x10C, lbl_eu_80664748, lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x11B, lbl_eu_80664748, lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x12A, lbl_eu_80664748, lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x139, lbl_eu_80664748, lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x148, lbl_eu_80664748, lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x157, lbl_eu_80664748, lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x166, lbl_eu_80664748, lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x175, lbl_eu_80664748, lbl_eu_80664750);
}

void func_802369C0(){}

void func_80236CF4(){}

void func_80236DB8(){}

void func_80236DF0(){}

void func_80236E28(){}

void func_80236E6C(){}

void func_80237050(){}

void func_802370A8(){}

void func_80237100(){}

void func_8023719C(){}

void func_80237238(){}

void func_80237394(){}

void func_802374F0(){}

void func_8023754C(){}

void func_802375A8(){}

void func_80237A0C(){}

void func_80237B88(){}

void func_80237D58(){}

void func_80237E24(){}

void func_80238038(){}

void func_80238298(){}

void func_802384F4(){}

void func_80238904(){}

void func_80239030(){}

void func_8023916C(){}

void func_8023939C(){}

void func_8023959C(){}

void func_802397F4(){}

void func_80239964(){}

void func_80239AA0(){}

void func_80239BDC(){}

void func_80239D20(){}

void func_80239EFC(){}

void func_80239FC4(){}

void func_8023A148(){}

void func_8023A210(){}

void func_8023A2D8(){}

void func_8023A398(){}

void func_8023A460(){}

void func_8023A55C(){}

void func_8023A60C(){}

void func_8023A6BC(){}

void func_8023A76C(){}

void func_8023A81C(){}

void func_8023A8CC(){}

void func_8023A97C(){}

void func_8023AA2C(){}

void func_8023AADC(){}

void func_8023AB8C(){}

void func_8023AD5C(){}

void func_8023AE24(){}

void func_8023AF60(){}

void func_8023B074(){}

void func_8023B12C(){}

void func_8023B280(){}

void func_8023B368(){}

void func_eu_8023D490(){}

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
void sinit_8023BC8C(){}
