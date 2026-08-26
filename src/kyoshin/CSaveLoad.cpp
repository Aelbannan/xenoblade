// Auto-scaffolded catalog TU for kyoshin/CSaveLoad
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CSaveLoad.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkSystem.hpp"
#include "monolib/lib/CLibHbm.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
// code_80135FDC.hpp declares several nw4r::db warning strings as u8[] while
// CfObjectImplMove.hpp (via harness_catalog.hpp) already declared some of them
// const char[]. This TU never uses those strings, so alias them away for the
// duration of the include.
#define lbl_eu_80526324 lbl_eu_80526324_decl_shield
#define lbl_eu_80526300 lbl_eu_80526300_decl_shield
#define lbl_eu_805262F0 lbl_eu_805262F0_decl_shield
#define lbl_eu_805262C8 lbl_eu_805262C8_decl_shield
#define lbl_eu_8052CB40 lbl_eu_8052CB40_decl_shield
#define lbl_eu_8052CB1C lbl_eu_8052CB1C_decl_shield
#define lbl_eu_8052E4E4 lbl_eu_8052E4E4_decl_shield
#define lbl_eu_8052E4C0 lbl_eu_8052E4C0_decl_shield
#define lbl_eu_8052E558 lbl_eu_8052E558_decl_shield
#define lbl_eu_8052E530 lbl_eu_8052E530_decl_shield
#include "kyoshin/code_80135FDC.hpp"
#undef lbl_eu_80526324_decl_shield
#undef lbl_eu_80526300_decl_shield
#undef lbl_eu_805262F0_decl_shield
#undef lbl_eu_805262C8_decl_shield
#undef lbl_eu_8052CB40_decl_shield
#undef lbl_eu_8052CB1C_decl_shield
#undef lbl_eu_8052E4E4_decl_shield
#undef lbl_eu_8052E4C0_decl_shield
#undef lbl_eu_8052E558_decl_shield
#undef lbl_eu_8052E530_decl_shield
#include "monolib/work/CEventFile.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include <string.h>
#include <revolution/os/OSCache.h>
#include "monolib/device/CDeviceSC.hpp"

// External function declarations
void func_801F369C(void*);
void func_80138078(u32);
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
// func_80137510 declared in code_80135FDC.hpp
extern const float lbl_eu_80668B68;
extern const float lbl_eu_80668B6C;
extern const float lbl_eu_80668B70;
extern const float lbl_eu_80668B74;

// Forward declarations for functions called before their definitions
// extern "C" to match retail unmangled reloc names
extern cf::CfAward lbl_eu_806649F8;
extern u8 lbl_eu_80576CF8[];
extern void __register_global_object(void*, void*, void*);
extern void func_8009D414(void*);

// CDeviceFile / MemManager / CSysWin / game system function declarations
extern u32 lbl_eu_80663E28;
extern u8 lbl_eu_805387B8[];
// External function declarations needed by OnFileEvent
// Declared as void* by kyoshin/cf/CfGameManager.hpp (via CWorkSystem.hpp)
extern void* lbl_eu_806640A8;

// Struct with three heap pointers at offsets 0, 4, 8
struct UnkStruct_3Ptr {
    void* p0;
    void* p4;
    void* p8;
};

extern u32 lbl_eu_806649F0;

// Allocate three 0xA000-byte buffers from MEM2 and store globally
#pragma optimize_for_size on
extern "C" void func_8028E838(UnkStruct_3Ptr* p) {
    u32 ptrVal;
    ptrVal = (u32)p;
    lbl_eu_806649F0 = ptrVal;
    p->p0 = allocate_head__Q23mtl10MemManagerFUlUli((u32)getHandleMEM2__Q23mtl10MemManagerFv(), 0xA000, 0x40);
    p->p4 = allocate_head__Q23mtl10MemManagerFUlUli((u32)getHandleMEM2__Q23mtl10MemManagerFv(), 0xA000, 0x40);
    p->p8 = allocate_head__Q23mtl10MemManagerFUlUli((u32)getHandleMEM2__Q23mtl10MemManagerFv(), 0xA000, 0x40);
}
#pragma optimize_for_size off

extern "C" __declspec(noinline) void func_8028E8A4(UnkStruct_3Ptr* p) {
    mtl::MemManager::deallocate(p->p0);
    mtl::MemManager::deallocate(p->p4);
    mtl::MemManager::deallocate(p->p8);
    lbl_eu_806649F0 = 0;
}

#pragma optimize_for_size on
extern "C" void func_8028E8EC(CSLCur* p) {
    // Call func_8023FA64 for each of the 3 heap pointers with the callback
    func_8023FA64(p->mField0, 1, func_8028E9E0);
    func_8023FA64(p->mField4, 2, func_8028E9E0);
    func_8023FA64(p->mField8, 3, func_8028E9E0);
    // Zero out the 6 byte fields at offsets 0x0C-0x11
    p->mFieldC = 0;
    p->mFieldD = 0;
    p->mFieldE = 0;
    p->_pad_0F = 0;
    p->mField10 = 0;
    p->mField11 = 0;
}
#pragma optimize_for_size off

u8 CSaveLoad::func_8028F664() { return mField123; }

extern u8 lbl_eu_8053884C[];

#pragma push
#pragma dont_inline on
extern "C" void __ct__CSLCur(CSLCur* self, int val) {
    // Store vtable-like pointer, parameter val, and zero everything else
    self->mField0 = (void*)&lbl_eu_8053884C;
    self->mField4 = reinterpret_cast<void*>(static_cast<uintptr_t>(val));
    self->mField8 = nullptr;
    // Word write at +0xC covers mFieldC/D/E/_pad_0F
    *reinterpret_cast<u32*>(&self->mFieldC) = 0;
    // Word write at +0x10 covers mField10-mField13
    *reinterpret_cast<u32*>(&self->mField10) = 0;
    self->mField14 = 0;
    self->mField15 = 0;
}
#pragma pop

// Reset a CSLCur to default state with mFieldC/D/E = 1
// dont_inline: retail calls this out-of-line from the constructor.
#pragma push
#pragma dont_inline on
extern "C" void func_8028E7C8(CSLCur* self) {
    self->mField0 = nullptr;
    self->mField4 = 0;
    self->mField8 = nullptr;
    self->mFieldC = 1;
    self->mFieldD = 1;
    self->mFieldE = 1;
    self->_pad_0F = 0;
    self->mField10 = 0;
    self->mField11 = 0;
}
#pragma pop

// Struct with a pointer at +8 that has virtual functions
struct UnkPtrHolder {
    char _pad_00[0x08];
    UnkPtrObj* mPtr;       // +0x08 - pointer to object with virtual functions
    void* mField0C;     // +0x0C - pointer to object with float field at +0x10
    void* mField10;     // +0x10 - AnimTransform*
    u8 mField14;      // +0x14
    u8 mField15;      // +0x15 - state byte (0=idle, 1=active)
};

// func_8028EC74 is a C-linkage function in retail (unmangled symbol); declare
// it extern "C" so the call reloc inside func_8028EC28 resolves unmangled.

// Declarations for functions called by func_8028EA74
void func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
    nw4r::lyt::Layout** ppLayout,
    nw4r::lyt::ArcResourceAccessor* accessor,
    const char* name);
void func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
    nw4r::lyt::Layout* layout,
    nw4r::lyt::AnimTransform** ppAnimTrans,
    nw4r::lyt::ArcResourceAccessor* accessor,
    char* name);

// optimize_for_size gives the retail stmw r30 / lmw r30 save-restore pair
#pragma push
#pragma optimize_for_size on
void CSLCur::func_8028EA74() {
    // Create layout, then bind two anim transforms and finish via layout vtable
    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        &mField8,
        (nw4r::lyt::ArcResourceAccessor*)mField4,
        (const char*)&lbl_eu_8050F7CC);

    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        mField8,
        (nw4r::lyt::AnimTransform**)&mFieldC,
        (nw4r::lyt::ArcResourceAccessor*)mField4,
        (char*)&lbl_eu_8050F7CC[0x18]);

    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        mField8,
        (nw4r::lyt::AnimTransform**)&mField10,
        (nw4r::lyt::ArcResourceAccessor*)mField4,
        (char*)&lbl_eu_8050F7CC[0x35]);

    // Virtual call at vtable offset 0x24 (slot 9) on the layout
    ((LayoutProxy24*)mField8)->vf9();

    func_8028EC74((UnkPtrHolder*)this);
}
#pragma pop

extern "C" __declspec(noinline) void func_8028EAF8(UnkPtrHolder* self) {
    if (self->mPtr == nullptr) return;

    int state = self->mField15;
    if (state == 0) goto case0;
    if (state == 1) goto case1;
    goto end;

case0:
    func_80137444((nw4r::lyt::AnimTransform*)self->mField0C, lbl_eu_80668B68);
    goto end;

case1:
    {
        void (*fn)(UnkPtrHolder*) = func_8028EC28;
        fn(self);
    }

end:
    ((UnkVtblObj*)self->mPtr)->vf14(0);
}

// Store 1 at ptr[index+0xB] and value at ptr[index+0xE]
// The global lbl_eu_806649F0 is a pointer to an array of struct-like slots
// Pointer-arithmetic form ((u8*)lbl + index) is load-bearing: the plain
// *(u8*)(lbl + index + off) form makes MWCC emit `add r3, r4, r0` for the
// second store (index first); the pointer form emits retail's `add r3, r0, r4`
// for both (a partial commutative-add swap breaks the witness rho).
extern "C" void func_8028E9E0(int /*unused*/, int index, u8 value) {
    if (lbl_eu_806649F0 == 0) return;
    ((u8*)lbl_eu_806649F0 + index)[0xB] = 1;
    ((u8*)lbl_eu_806649F0 + index)[0xE] = value;
}

void func_80137038(
    nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

// Render layout with null + active guards; tail-calls func_80137038
// noinline: retail keeps this out-of-line (called from func_8028F3D4).
extern "C" __declspec(noinline) void func_8028EB70(CSLCur* self, nw4r::lyt::DrawInfo* drawInfo) {
    nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)self->mField8;
    if (layout == nullptr) return;
    if (self->mField14 == 0) return;
    func_80137038(layout, drawInfo, 0, 1);
}

extern "C" __declspec(noinline) void func_8028EB9C(UnkPtrHolder* self) {
    void* ptr = self->mPtr;
    self->mField14 = 0;
    self->mField0C = 0;
    self->mField10 = 0;

    if (ptr != nullptr) {
        // Redundant inner null check: MWCC CSEs the repeated test into one cmpi
        // and re-tests it, reproducing the retail's second (dead) beq.
        if (ptr != nullptr) {
            // Virtual call at vtable offset 0x8 (slot 2 after RTTI pads),
            // deleting-destructor pattern with arg 1.
            ((UnkObj*)ptr)->vf2(1);
        }
        self->mPtr = nullptr;
    }
}

// Retail symbol is unmangled; C linkage required for correct call relocs.
extern "C" void func_8028EC04(void* arg1, const void* arg2) {
    void* ptr1 = *(void**)((char*)arg1 + 8);
    void* ptr2 = *(void**)((char*)ptr1 + 0x10);
    const float* src = (const float*)arg2;
    float* dst = (float*)((char*)ptr2 + 0x2c);
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

#pragma dont_inline on
extern "C" void func_8028EC28(UnkPtrHolder* self) {
    const float f = lbl_eu_80668B68;
    if (func_80137444((nw4r::lyt::AnimTransform*)self->mField10, f) != 0) {
        self->mField15 = 0;
        // Use function pointer to prevent inlining
        void (*fn)(UnkPtrHolder*) = func_8028EC74;
        fn(self);
    }
}
#pragma dont_inline off

void func_8028ED0C(CSLCur* data, int r4) {
    data->mField0 = (void*)(uintptr_t)r4;
    data->mField4 = 0;
    data->mField8 = 0;
    data->mFieldC = 0;
    data->mFieldD = 0;
    data->mFieldE = 1;
}


extern "C" void* __dt__8028E7F8(void* self, int dealloc_flag) {
    if (self != nullptr && dealloc_flag > 0) {
        __dl__FPv(self);
    }
    return self;
}

extern "C" void* __dt__8028EA34(void* self, int dealloc_flag) {
    if (self != nullptr && dealloc_flag > 0) {
        __dl__FPv(self);
    }
    return self;
}

extern "C" void* __dt__8028ED30(void* self, int dealloc_flag) {
    if (self != nullptr && dealloc_flag > 0) {
        __dl__FPv(self);
    }
    return self;
}

struct UnkTwoPtr;

// Abstract class to match the font object's vtable layout
// The vtable has 2 MWCC implicit entries (RTTI/dtor) before user virtuals
// Slot 9 (offset 0x24) returns a u32 value used by func_8013676C
class FontHelper {
public:
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual u32 v7() = 0; // vtable offset 0x24
};

// Layout initialization: create layout, anim transform, and set up font
// This is a C-linkage function (retail symbol is unmangled). optimize_for_size
// for the retail stmw r30/lmw r30 save pair.
#pragma push
#pragma optimize_for_size on
extern "C" void func_8028ED70(UnkED70_Struct* s) {
    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        &s->mLayout, s->mAccessor, (const char*)&lbl_eu_8050F7CC[0x50]);
    
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        s->mLayout, &s->mAnimTrans, s->mAccessor, (char*)&lbl_eu_8050F7CC[0x69]);
    
    // Load root pane from Layout + 0x10 (direct field access, not GetRootPane virtual call)
    void* rootPane = *(void**)((char*)s->mLayout + 0x10);
    void* fontObj = CDeviceFont::func_80452C10(1, s->mLayout);
    
    // Virtual call on fontObj at vtable slot 9 (offset 0x24, retail r12 two-step)
    u32 result = static_cast<FontHelper*>(fontObj)->v7();
    func_8013676C((nw4r::lyt::Pane*)rootPane, result);

    func_8028EF74((UnkTwoPtr*)s);
}
#pragma pop

// Dispatch to cursor animation update based on mFieldC state,
// then call virtual slot 14 on the mField4 object
extern "C" __declspec(noinline) void func_8028EDF8(CSLCur* self) {
    if (self->mField4 == 0) return;

    switch (self->mFieldC) {
    case 1:
        // Function pointer cast prevents inlining
        ((void (*)(CSLCur*))func_8028EED8)(self);
        break;
    case 3:
        ((void (*)(CSLCur*))func_8028EF24)(self);
        break;
    }

    // Virtual call at vtable slot 14 (0x38) on the mField4 object
    ((UnkVtblObj*)(uintptr_t)self->mField4)->vf14(0);
}

// Struct with a pointer at +4 that has a vtable
struct UnkSlot4Ptr {
    void* mField0;  // +0x00
    void* mPtr;     // +0x04 - pointer to an object with vtable
};

extern "C" __declspec(noinline) void func_8028EE68(UnkSlot4Ptr* self) {
    UnkObj* ptr = (UnkObj*)self->mPtr;
    if (ptr == nullptr) {
        return;
    }
    if (ptr != nullptr) {
        // Virtual destructor call at vtable slot 2 with delete flag 1
        ptr->vf2(1);
    }
    self->mPtr = nullptr;
}

// Retail defines this helper under an unmangled (C) symbol name.
extern "C" __declspec(noinline) void func_8028EEC0(CSLCur* self) {
    self->mFieldC = 1;
    self->mFieldD = 1;
    self->mFieldE = 0;
}
extern "C" void func_8028EED8(CSLCur* cur) {
    const float f = lbl_eu_80668B68;
    if (func_80137444((nw4r::lyt::AnimTransform*)cur->mField8, f) != 0) {
        cur->mFieldC = 2;
        cur->mFieldE = 1;
    }
}

extern "C" void func_8028EF24(CSLCur* self) {
    if (func_80137510((nw4r::lyt::AnimTransform*)self->mField8, lbl_eu_80668B68) != 0) {
        self->mFieldC = 0;
        self->mFieldD = 0;
        self->mFieldE = 1;
    }
}

// Struct with two pointer fields at +4 and +8
struct UnkTwoPtr {
    char _pad_00[0x04];
    UnkVtblObj* mObjA;  // +0x04 - object with virtuals at vtable 0x2C / 0x38
    void* mObjB;        // +0x08 - object with a float field at +0x10
};

extern "C" __declspec(noinline) void func_8028EF74(UnkTwoPtr* self) {
    ((UnkVtblObj*)self->mObjA)->vf11(self->mObjB, 1);
    *(float*)((char*)self->mObjB + 0x10) = lbl_eu_80668B6C;
    ((UnkVtblObj*)self->mObjA)->vf14(0);
}

// CSaveLoad constructor (retail symbol __ct__CSaveLoad).
// Builds the save/load screen: sub-object constructors run first, state
// fields are initialized, then three temporaries (one CScrollBar, two
// CSysWin) are constructed and copied into the embedded members.
// optimize_for_size reproduces the retail _savegpr_28/_restgpr_28 prologue.
#pragma push
#pragma optimize_for_size on
extern "C" CSaveLoad* __ct__CSaveLoad(CSaveLoad* self, int arg4, int arg5) {
    // Block-scope externs inherit the retail C-linkage symbols.
    extern void __ct__17UnkClass_8045F564Fv(void*);
    extern void __ct__CSLCur(CSLCur*, int);
    extern void __ct__CCur18(void*, int);
    extern void __ct__CScrollBar(void*, int);
    extern void __ct__CSysWin(void*, int);
    extern void func_8028ED0C(void*, int);
    extern void func_8028E7C8(CSLCur*);
    extern void __ct__UnkClass_8011C974(void*, void*);
    extern void __dt__10CScrollBarFv(void*, int);
    extern void func_8016742C(void*, void*);
    extern void __dt__7CSysWinFv(void*, int);

    // Temp storage; declaration order fixes stack offsets (scrollbar
    // @+0x80, arg-2 win @+0x44, arg-0 win @+0x08 in the retail frame).
    u8 tempScrollbar[0x40];
    u8 tempSysWin2[0x3C];
    u8 tempSysWin0[0x3C];

    // Set vtable at offset 0x00
    *(void**)self = (void*)lbl_eu_805387B8;

    __ct__17UnkClass_8045F564Fv((u8*)self + 0x04);

    self->mFileHandle = nullptr;
    self->mArcAccessor = nullptr;
    self->mLayout = nullptr;
    self->mAnimTransA = nullptr;
    self->mAnimTransB = nullptr;

    __ct__CSLCur((CSLCur*)((u8*)self + 0x28), 0);

    __ct__CCur18((u8*)self + 0x40, 0);

    __ct__CScrollBar(self->mScrollbar, 0);

    __ct__CSysWin(&self->mSysWin98, 0);
    __ct__CSysWin(&self->mSysWinD4, 0);

    func_8028ED0C((u8*)self + 0x110, 0);

    self->mField120 = 0;
    self->mField121 = 0;
    self->mField122 = 0;
    self->mField123 = 1;
    self->mField124 = 0;
    self->mField126 = 0;
    self->mField128 = 1;
    self->mField129 = (u8)arg4;
    self->mField12A = 0;
    self->mField12B = (u8)arg5;
    self->mField12C = 0;
    self->mField12D = 0;
    self->mField12E = 0;
    self->mField12F = 0;

    func_8028E7C8(&self->mCur);

    self->mField130 = nullptr;
    self->mField134 = nullptr;
    self->mField138 = nullptr;

    // Temporary CScrollBar for copy initialization
    __ct__CScrollBar(tempScrollbar, 1);

    __ct__UnkClass_8011C974((u8*)self + 0x5C, tempScrollbar + 4);

    // Copy scalar tail from tempScrollbar+0x14 into self+0x6C
    *(u32*)((u8*)self + 0x6C) = *(u32*)(tempScrollbar + 0x14);
    *(u32*)((u8*)self + 0x70) = *(u32*)(tempScrollbar + 0x18);
    *(u32*)((u8*)self + 0x74) = *(u32*)(tempScrollbar + 0x1C);
    *(u32*)((u8*)self + 0x78) = *(u32*)(tempScrollbar + 0x20);
    *(u8*)((u8*)self + 0x7C) = *(u8*)(tempScrollbar + 0x24);
    *(u8*)((u8*)self + 0x7D) = *(u8*)(tempScrollbar + 0x25);
    *(u8*)((u8*)self + 0x7E) = *(u8*)(tempScrollbar + 0x26);
    *(u8*)((u8*)self + 0x7F) = *(u8*)(tempScrollbar + 0x27);
    *(float*)((u8*)self + 0x80) = *(float*)(tempScrollbar + 0x28);
    *(float*)((u8*)self + 0x84) = *(float*)(tempScrollbar + 0x2C);
    *(float*)((u8*)self + 0x88) = *(float*)(tempScrollbar + 0x30);
    *(float*)((u8*)self + 0x8C) = *(float*)(tempScrollbar + 0x34);
    *(float*)((u8*)self + 0x90) = *(float*)(tempScrollbar + 0x38);
    *(u8*)((u8*)self + 0x94) = *(u8*)(tempScrollbar + 0x3C);

    __dt__10CScrollBarFv(tempScrollbar, -1);

    // Temporary CSysWin for copy initialization of mSysWin98
    __ct__CSysWin(tempSysWin2, 2);
    func_8016742C(&self->mSysWin98, tempSysWin2);
    __dt__7CSysWinFv(tempSysWin2, -1);

    // Temporary CSysWin for copy initialization of mSysWinD4
    __ct__CSysWin(tempSysWin0, 0);
    func_8016742C(&self->mSysWinD4, tempSysWin0);
    __dt__7CSysWinFv(tempSysWin0, -1);

    return self;
}
#pragma pop

// CSaveLoad destructor. MWCC compiles this member dtor to the retail symbol
// __dt__9CSaveLoadFv with the standard (this, flags) ABI.
#pragma optimize_for_size on
CSaveLoad::~CSaveLoad() {
    CSaveLoad* self = this;
    int flags = 0;
    if (self != nullptr) {
        __dt__7CSysWinFv((u8*)self + 0xD4, -1);
        __dt__7CSysWinFv((u8*)self + 0x98, -1);
        __dt__10CScrollBarFv((u8*)self + 0x58, -1);
        __dt__6CCur18Fv((u8*)self + 0x40, -1);
        __dt__17UnkClass_8045F564Fv((u8*)self + 0x04, -1);
        if (flags > 0) {
            __dl__FPv((u8*)self);
        }
    }
}
#pragma optimize_for_size off

void CSaveLoad::func_8028F23C() {
    u32 handle = (u32)getHandleMEM2__Q23mtl10MemManagerFv();
    mFileHandle = (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(handle, (const char*)&lbl_eu_8050F7CC[0x92], this, 0, 0);
    func_801F34F4(mScrollbar);

    // Virtual call on CSysWin-like objects at offsets 0x98 and 0xD4
    // Use virtual dispatch through CSysWinProxy to match retail r12 pattern
    // v32 is at vtable slot 34 (offset 0x88) after 2 implicit entries
    ((CSysWinProxy*)(&mSysWin98))->v32();
    ((CSysWinProxy*)(&mSysWinD4))->v32();

    mField122 = 0;
    lbl_eu_806649F4 = this;
    lbl_eu_80662AD0 = -1;
}


// Draw the save/load screen layout and all sub-windows.
// optimize_for_size gives the retail stmw r30 / lmw r30 save-restore pair.
#pragma push
#pragma optimize_for_size on
extern "C" void func_8028F3D4(CSaveLoad* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->mField120 == 0) return;

    func_80137038(self->mLayout, drawInfo, 0, 1);
    func_801F35B0(self->mScrollbar, drawInfo);

    // Cursor pane is only drawn while both windows are idle
    if (CSysWin_getUnk34(&self->mSysWin98) == 0 &&
        CSysWin_getUnk34(&self->mSysWinD4) == 0 &&
        self->mField11D == 0 &&
        self->mField121 <= 5) {
        func_8028EB70((CSLCur*)((char*)self + 0x28), drawInfo);
    }

    func_8022B7C8(&self->mSysWin98, drawInfo);
    func_801D20B0((char*)self + 0x40, drawInfo);
    func_8022B7C8(&self->mSysWinD4, drawInfo);

    // Sub-layout pointer stored inside the second CSysWin block (+0x114)
    nw4r::lyt::Layout* subLayout = *(nw4r::lyt::Layout**)((char*)&self->mSysWinD4 + 0x40);
    if (subLayout != nullptr) {
        func_80137038(subLayout, drawInfo, 0, 1);
    }
}
#pragma pop


// Cleanup/reset function for CSaveLoad
void func_8028F4AC(CSaveLoad* self) {
    CWorkSystem::setSaveLoadInvalidReset(false);
    func_eu_804521BC(1);
    CLibHbm::func_8045D470(false);
    func_801390E0(&self->mFileHandle);
    self->mField120 = 0;
    // Use local variable to prevent MWCC from optimizing &self->mCur to a load
    UnkStruct_3Ptr* curPtr = reinterpret_cast<UnkStruct_3Ptr*>(&self->mCur);
    func_8028E8A4(curPtr);
    
    if (self->mLayout != nullptr) {
        if (self->mLayout != nullptr) {
            ((UnkObj*)self->mLayout)->vf2(1);
        }
        self->mLayout = nullptr;
    }
    
    func_80139124(self->mArcAccessor);
    
    if (self->mField130 != nullptr) {
        mtl::MemManager::deallocate(self->mField130);
        self->mField130 = nullptr;
    }
    if (self->mField134 != nullptr) {
        mtl::MemManager::deallocate(self->mField134);
        self->mField134 = nullptr;
    }
    if (self->mField138 != nullptr) {
        mtl::MemManager::deallocate(self->mField138);
        self->mField138 = nullptr;
    }
    
    func_8028EB9C((UnkPtrHolder*)((char*)self + 0x28));
    func_801F35DC((u8*)self + 0x58);
    func_8022B7F4(&self->mSysWin98);
    // Virtual call at self+0x40 (CCur18), vtable slot 3 (offset 0x0C), no null check
    ((CCur18Obj*)((char*)self + 0x40))->vf3();
    func_8022B7F4(&self->mSysWinD4);
    func_8028EE68((UnkSlot4Ptr*)((char*)self + 0x110));
    ((UnkClass_8045F564*)((char*)self + 0x04))->func_8045F778();
    lbl_eu_806649F4 = nullptr;
}

u8 func_8028F5C4(CSaveLoad* p) {
    if (!CScrollBar_isVisible(p->mScrollbar)) return 0;
    if (!CSysWin_isReady((void*)&p->mSysWin98)) return 0;
    if (!CSysWin_isReady((void*)&p->mSysWinD4)) return 0;
    // Use function pointer to prevent inlining
    {
        int (*checkFn)(CSLCur*) = func_8028E964;
        if (!checkFn(&p->mCur)) return 0;
    }
    if (p->mField11D != 0) return 0;
    return p->mField122;
}


// Check save window state: returns 1 if window is active/accessible,
// or mField11D if both windows are idle
u8 func_8028F66C(CSaveLoad* p) {
    if (p->mField121 != 3) return 1;
    if (CSysWin_getUnk34((void*)&p->mSysWin98) != 0) return 1;
    if (CSysWin_getUnk34((void*)&p->mSysWinD4) != 0) return 1;
    return p->mField11D;
}

void func_8028F6DC(CSaveLoad* p) {
    if (p->mField121 != 0) return;

    p->mField121 = 1;
    p->mField123 = 0;
    // Use function pointers to prevent inlining
    {
        void (*fn907E4)(CSaveLoad*) = func_802907E4;
        void (*fn910D4)(CSaveLoad*) = func_802910D4;
        void (*fn908A4)(CSaveLoad*) = func_802908A4;
        fn907E4(p);
        fn910D4(p);
        fn908A4(p);
    }

    // Set up float array for scrollbar init
    float vec[3];
    vec[0] = lbl_eu_80668B70;
    vec[1] = lbl_eu_80668B74;
    vec[2] = lbl_eu_80668B6C;
    func_801F3670(p->mScrollbar, vec);

    int val = (int)(u16)lbl_eu_80662AC8;
    func_801F36BC(p->mScrollbar, 3, val);

    {
        void (*fn90994)(CSaveLoad*) = func_80290994;
        fn90994(p);
    }
}

#pragma optimize_for_size on
extern "C" void func_8028F774(CSaveLoad* p) {
    if (p->mField121 != 3) return;
    p->mField121 = 4;
    p->mField123 = 0;
    // Function pointer cast prevents inlining of func_80290844's body
    ((void (*)(CSaveLoad*))func_80290844)(p);
    p->mField3C = 0;
    func_801F369C(p->mScrollbar);
    func_80138078(6);
}
#pragma optimize_for_size off

// Handle cursor up/left movement in save/load screen.
// Decrements cursor position with wrap-around logic for both rows and columns.
void func_8028F7D0(CSaveLoad* self) {
    if (self->mField121 != 3) return;
    if (CSysWin_getUnk34(&self->mSysWinD4) != 0) return;
    if (self->mField11D != 0) return;
    
    if (CSysWin_getUnk34(&self->mSysWin98) != 0) {
        if (CSysWin_isActive(&self->mSysWin98) == 0) return;
        
        // Decrement mField128 with wrap from 0 to 1
        u8 val = self->mField128 - 1;
        self->mField128 = val;
        if ((s8)val < 0) {
            self->mField128 = 1;
        }
        
        func_802908A4(self);
        func_80138078(1);
        return;
    }
    
    // Decrement mField124
    u8 f124 = self->mField124 - 1;
    self->mField124 = f124;
    if ((s8)f124 >= 0) goto normalPath;
    
    // Underflow: move to previous row
    // Signed predecrement of the field: MWCC loads with lha, decrements in
    // place, stores, and sign-extends into a scratch reg for the test
    // (subi/extsh./sth on one register), matching retail.
    self->mField124 = 0;
    if (--self->mField126 >= 0) goto normalPath;
    
    // Underflow again: wrap to last row
    {
        s32 maxRows = (s32)lbl_eu_80662AC8;
        if (maxRows >= 3) {
            self->mField124 = 2;
            self->mField126 = (u16)((s32)lbl_eu_80662AC8 - 3);
        } else {
            u8 prev = (u8)(maxRows - 1);
            self->mField124 = prev;
            self->mField126 = 0;
            if ((s8)prev < 0) {
                self->mField124 = 0;
            }
        }
    }
    
normalPath:
    func_802908A4(self);
    func_801F3850(self->mScrollbar, self->mField126);
    func_80138078(1);
}

// Handle cursor down/right movement in save/load screen.
// Increments cursor position with wrap-around logic.
void func_8028F904(CSaveLoad* self) {
    if (self->mField121 != 3) return;
    if (CSysWin_getUnk34(&self->mSysWinD4) != 0) return;
    if (self->mField11D != 0) return;
    
    if (CSysWin_getUnk34(&self->mSysWin98) != 0) {
        if (CSysWin_isActive(&self->mSysWin98) == 0) return;
        
        u8 val = self->mField128 + 1;
        self->mField128 = val;
        if ((s8)val > 1) {
            self->mField128 = 0;
        }
        
        func_802908A4(self);
        func_80138078(1);
        return;
    }
    
    // Main scrollbar path
    if (lbl_eu_80662AC8 >= 3) {
        u8 f124;
        f124 = self->mField124 + 1;
        self->mField124 = f124;
        if ((s8)f124 >= 3) {
            s16 row;
            self->mField124 = 2;
            row = (s16)self->mField126 + 1;
            self->mField126 = (u16)row;
            if (row > (lbl_eu_80662AC8 - 3)) {
                self->mField124 = 0;
                self->mField126 = 0;
            }
            goto updatePath;
        }
    } else {
        u8 f124;
        f124 = self->mField124 + 1;
        self->mField124 = f124;
        if ((s8)f124 >= lbl_eu_80662AC8) {
            self->mField124 = 0;
            self->mField126 = 0;
        }
    }

updatePath:
    func_802908A4(self);
    func_801F3850(self->mScrollbar, self->mField126);
    func_80138078(1);
}

// Handle cursor movement: if global count >= 3, move cursor back by 3 positions
// with underflow handling. Otherwise reset cursor to 0.
void func_8028FA54(CSaveLoad* p) {
    if (p->mField121 != 3) return;
    if (CSysWin_getUnk34((void*)&p->mSysWin98) != 0) return;
    if (CSysWin_getUnk34((void*)&p->mSysWinD4) != 0) return;
    if (p->mField11D != 0) return;

    if ((s32)lbl_eu_80662AC8 >= 3) {
        s16 sval = (s16)p->mField126;
        s16 newVal = sval - 3;
        p->mField126 = (u16)newVal;
        if (newVal < 0) {
            int adj = (int)newVal + 2;
            p->mField124 = (u8)adj;
            p->mField126 = 0;
            if ((s8)(u8)adj < 0) {
                p->mField124 = 0;
            }
        }
    } else {
        p->mField124 = 0;
        p->mField126 = 0;
    }

    func_802908A4(p);
    func_801F3850(p->mScrollbar, p->mField126);
    func_80138078(1);
}

// Page-down / advance the save-slot cursor by one entry (wraps every 3 rows).
extern "C" void func_8028FB20(CSaveLoad* self) {
    if (self->mField121 != 3) return;
    if (CSysWin_getUnk34(&self->mSysWin98) != 0) return;
    if (CSysWin_getUnk34(&self->mSysWinD4) != 0) return;
    if (self->mField11D != 0) return;

    if ((s32)lbl_eu_80662AC8 >= 3) {
        // Advance row cursor by 3; clamp/wrap when past the last page
        s16 newVal = (s16)self->mField126 + 3;
        self->mField126 = (u16)newVal;
        if ((int)newVal > lbl_eu_80662AC8 - 3) {
            // Wrapped past the final row: derive column and clamp to 2
            u8 diff = (u8)(newVal - (lbl_eu_80662AC8 - 3));
            self->mField124 = diff;
            self->mField126 = (u16)(lbl_eu_80662AC8 - 3);
            if ((s8)diff >= 3) {
                self->mField124 = 2;
            }
        }
    } else {
        // Decrement last column; wrap to previous row on underflow
        int prev = lbl_eu_80662AC8 - 1;
        u8 prevU8 = prev;
        self->mField124 = prev;
        self->mField126 = 0;
        if ((s8)prevU8 < 0) {
            self->mField124 = 0;
        }
    }

    func_802908A4(self);
    func_801F3850(self->mScrollbar, self->mField126);
    func_80138078(1);
}

// Handle window state transitions for save/load screen.
// Checks syswin at 0x98 and 0xD4, sets up cursor strings and state.
#pragma push
#pragma optimize_for_size on
void func_8028FC18(CSaveLoad* self) {
    char* s1;
    char* s2;
    char* s3;

    if (CSysWin_getUnk34(&self->mSysWin98) != 0) {
        if (CSysWin_isActive(&self->mSysWin98) != 0) {
            if ((s8)self->mField128 == 0) {
                func_80138078(3);
            } else {
                func_80138078(6);
                self->mField12C = 0;
            }
            func_8022B8E4(&self->mSysWin98);
            func_801D216C((void*)((u8*)self + 0x40), 0);
            self->mField121 = 7;
        }
        return;
    }

    if (CSysWin_getUnk34(&self->mSysWinD4) != 0) {
        if (CSysWin_isActive(&self->mSysWinD4) != 0) {
            func_8022B8E4(&self->mSysWinD4);
            self->mField121 = 9;
            if (self->mField12C != 0) {
                self->mField12C = 0;
            }
            func_80138078(3);
        }
        return;
    }

    if (self->mField121 != 3) return;

    // Function pointer cast prevents MWCC from inlining func_8028E998 (retail calls it)
    if (((int (*)(CSLCur*, u8))func_8028E998)(&self->mCur, self->mField124) == 0) goto noCursor;

    if (self->mField129 != 0) {
        s1 = (char*)func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x43);
        s2 = (char*)func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x44);
        s3 = (char*)func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x45);
        goto common;
    }

    s1 = (char*)func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x33);
    s2 = (char*)func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x34);
    s3 = (char*)func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x35);
    goto common;

noCursor:
    if (self->mField129 != 0) return;
    s1 = (char*)func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x38);
    s2 = (char*)func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x39);
    s3 = (char*)func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x3a);

common:
    func_8022B9B4(&self->mSysWin98, (u32)s1, 0);
    func_8022BF6C(&self->mSysWin98, (u32)s2, (u32)s3);
    func_8022BFC8(&self->mSysWin98, 0);
    func_8022B8B8(&self->mSysWin98);
    self->mField128 = 1;
    self->mField121 = 6;
    func_80138078(3);
}
#pragma pop

// Handle window close event: if the first syswin is active and ready,
// close it, reset the sub-object state, and update status fields
void func_8028FE50(CSaveLoad* p) {
    if (CSysWin_getUnk34((void*)&p->mSysWin98) == 0) return;
    if (CSysWin_isActive((void*)&p->mSysWin98) == 0) return;

    func_8022B8E4((void*)&p->mSysWin98);
    func_801D216C((char*)p + 0x40, 0);

    p->mField121 = 7;
    p->mField128 = 1;
    p->mField12C = 0;
}

u8 CSaveLoad::func_8028FEC4() { return mField12A; }

// Open the delete-confirm window for the selected slot.
#pragma optimize_for_size on
void func_8028FECC(CSaveLoad* self) {
    if (self->mField121 != 3) return;
    if (CSysWin_getUnk34(&self->mSysWin98) != 0) return;
    if (CSysWin_getUnk34(&self->mSysWinD4) != 0) return;
    if (self->mField11D != 0) return;
    if (((int (*)(CSLCur*, u8))func_8028E998)(&self->mCur, self->mField124) == 0) return;

    char* s1 = func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x3b);
    char* s2 = func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x3c);
    char* s3 = func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x3d);
    func_8022B9B4(&self->mSysWin98, (u32)s1, 0);
    func_8022BF6C(&self->mSysWin98, (u32)s2, (u32)s3);
    func_8022BFC8(&self->mSysWin98, 0);
    func_8022B8B8(&self->mSysWin98);
    self->mField128 = 1;
    self->mField12C = 1;
    self->mField121 = 6;
    func_80138078(3);
}
#pragma optimize_for_size off

// Check save/load state and return a status code:
// 0 = not ready (state mismatch, window active, or sub-window active)
// 0x69 = ready (no cursor) - mField129 == 0, func_8028E998 returns 0
// 0x6A = ready (has cursor) - mField129 == 0, func_8028E998 returns > 0
// 0x6B = ready (has cursor, mField129 != 0) - func_8028E998 returns > 0
int func_8028FFD4(CSaveLoad* p) {
    if (p->mField121 != 3) return 0;
    if (CSysWin_getUnk34((void*)&p->mSysWin98) != 0) return 0;
    if (CSysWin_getUnk34((void*)&p->mSysWinD4) != 0) return 0;
    if (p->mField11D != 0) return 0;

    if (p->mField129 != 0) {
        // Cursor active: return 0x6B if valid, 0 otherwise
        // Use function pointer to prevent inlining (retail uses direct bl but MWCC inlines it)
        int rv = ((int (*)(CSLCur*, u8))func_8028E998)(&p->mCur, p->mField124);
        return rv > 0 ? 0x6B : 0;
    } else {
        // Cursor inactive: return 0x6A if valid, 0x69 otherwise
        int rv = ((int (*)(CSLCur*, u8))func_8028E998)(&p->mCur, p->mField124);
        return rv > 0 ? 0x6A : 0x69;
    }
}

extern "C" __declspec(noinline) void func_80290094(CSaveLoad* p) {
    if (func_80137444(p->mAnimTransA, lbl_eu_80668B68) != 0) {
        p->mField121 = 2;
        // Use function pointer to prevent inlining
        void (*fn)(CSaveLoad*) = func_80290844;
        fn(p);
    }
}

extern "C" __declspec(noinline) void func_802900E0(CSaveLoad* p) {
    if (func_80137444(p->mAnimTransB, lbl_eu_80668B68) != 0) {
        p->mField121 = 3;
        p->mField3C = 1;
        func_802908A4(p);
        p->mField123 = 1;
    }
}

extern "C" __declspec(noinline) void func_80290140(CSaveLoad* self) {
    if (func_80137510(self->mAnimTransB, lbl_eu_80668B68) != 0) {
        self->mField121 = 5;
        {
            void (*fn)(CSaveLoad*) = func_802907E4;
            fn(self);
        }
    }
}

extern "C" __declspec(noinline) void func_8029018C(CSaveLoad* self) {
    if (func_80137510(self->mAnimTransA, lbl_eu_80668B68) != 0) {
        self->mField121 = 0;
        self->mField123 = 1;
    }
}

extern "C" __declspec(noinline) void func_802901D8(CSaveLoad* self) {
    if (CSysWin_isActive((void*)&self->mSysWin98) != 0) {
        self->mField121 = 3;
        func_801D216C((char*)self + 0x40, 1);
        {
            void (*fn)(CSaveLoad*) = func_802908A4;
            fn(self);
        }
    }
}

// Handle save/load operation execution (state 5).
// Checks window state, cursor status, and dispatches to delete/load/save logic.
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_8029022C(CSaveLoad* self) {
    if (CSysWin_isActive((void*)&self->mSysWin98) == 0) return;

    self->mField121 = 3;
    if ((s8)self->mField128 != 0) return;

    self->mField121 = 10;

    if (self->mField12C != 0) {
        // Delete/overwrite path
        CWorkSystem::setSaveLoadInvalidReset(true);
        func_eu_804521BC(0);
        CLibHbm::func_8045D470(true);

        char* r5 = (char*)func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x52);
        func_80136B4C(*(nw4r::lyt::Layout**)((u8*)self + 0x114), (char*)&lbl_eu_8050F7CC[0x85], r5, 0u);

        func_8028EEC0((CSLCur*)((u8*)self + 0x110));

        func_8023F860((s8)self->mField124 + 1, (void*)func_80291204);

        lbl_eu_80662AD0 = 0;
    } else if (self->mField129 != 0) {
        // Load path
        char* r5 = (char*)func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x46);
        func_80136B4C(*(nw4r::lyt::Layout**)((u8*)self + 0x114), (char*)&lbl_eu_8050F7CC[0x85], r5, 0u);

        func_8028EEC0((CSLCur*)((u8*)self + 0x110));

        func_8023FB28((s8)self->mField124 + 1, (void*)func_80291204);

        lbl_eu_80662AD0 = 2;

        // Retail runs the slot update only when the presentation bit is CLEAR
        // (rlwinm./bne skips over it otherwise).
        if ((lbl_eu_80663E28 & 0x01000000u) == 0) {
            int result = func_8028E998(&self->mCur, self->mField124);
            func_80083470__Q22cf13CfGameManagerFv(*(u16*)(result + 0x0E), *(u8*)(result + 0x11), 1);
        }
    } else {
        // Save path
        if (self->mField12B != 0) {
            func_8009D018(0x3213, 1);
        }

        CWorkSystem::setSaveLoadInvalidReset(true);
        func_eu_804521BC(0);
        CLibHbm::func_8045D470(true);

        char* r5 = (char*)func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x36);
        func_80136B4C(*(nw4r::lyt::Layout**)((u8*)self + 0x114), (char*)&lbl_eu_8050F7CC[0x85], r5, 0u);

        func_8028EEC0((CSLCur*)((u8*)self + 0x110));

        func_8023F3C0((s8)self->mField124 + 1, (void*)func_80291204, self->mField12B);

        lbl_eu_80662AD0 = 3;
    }

    func_80138078(0x80);
}
#pragma pop

extern "C" __declspec(noinline) void func_8029040C(CSaveLoad* p) {
    if (!CSysWin_isActive((void*)&p->mSysWinD4)) return;

    if (p->mField129 == 0) goto setError3;
    if (p->mField12D == 0) goto setError3;

    p->mField12A = 1;
    func_80145018();
    func_80142C80();
    func_8014A2D0();
    invalidateQstFlag();

    // Load global after function calls to prevent reordering
    {
        u32 flags = lbl_eu_80663E28;
        if ((flags & 0x01000000u) != 0) {
            getInstance__9CTaskGameFv();
            func_800426A8();
        } else {
            func_80084F50__Q22cf13CfGameManagerFv();
        }
    }
    return;

setError3:
    p->mField121 = 3;
}

extern "C" __declspec(noinline) void func_8029049C(CSaveLoad* p) {
    if (p->mField11E != 0) {
        p->mField121 = 3;
    }
}

// Handle save/load window state machine.
// Checks mField11E, mField12E, mField12C, mField129, mField12B
// and sets up the appropriate syswin with strings.
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_802904B4(CSaveLoad* self) {
    char* s1;
    char* s2;
    char* s3;

    if (self->mField11E == 0) return;

    self->mField121 = 8;

    if (self->mField12E != 0) {
        int val = self->mField12F;
        // Forward-goto structure mirrors retail layout:
        // dispatch, body1, body2, 3-string block, then mField12E=0
        if ((u32)(val - 3) <= 1u) goto threeString;
        if (val == 1) goto win54;
        if (val == 2) goto win55;
        goto clear12E;
win54:
        s1 = (char*)func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x54);
        func_8022B9B4(&self->mSysWinD4, (u32)s1, 0);
        func_8022BFC8(&self->mSysWinD4, 1);
        func_8022B8B8(&self->mSysWinD4);
        goto clear12E;
win55:
        s1 = (char*)func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x55);
        func_8022B9B4(&self->mSysWinD4, (u32)s1, 0);
        func_8022BFC8(&self->mSysWinD4, 1);
        func_8022B8B8(&self->mSysWinD4);
        goto clear12E;
threeString:
        // 3-string path: update syswin 0x98 with strings
        s1 = (char*)func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x56);
        s2 = (char*)func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x59);
        // Statement-form selection keeps MWCC branchy (cmpi/li/beq/li), matching retail
        int msgIdx;
        if (self->mField129 != 0)
            msgIdx = 0x58;
        else
            msgIdx = 0x57;
        // Single call site
        s3 = (char*)func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2],
                                  msgIdx);
        func_8022B9B4(&self->mSysWin98, (u32)s1, 0);
        func_8022BF6C(&self->mSysWin98, (u32)s2, (u32)s3);
        func_8022BFC8(&self->mSysWin98, 0);
        func_8022B8B8(&self->mSysWin98);
        self->mField128 = 1;
        self->mField12C = 1;
        self->mField121 = 6;
clear12E:
        self->mField12E = 0;
        return;
    }

    if (self->mField12C != 0) {
        CWorkSystem::setSaveLoadInvalidReset(false);
        func_eu_804521BC(1);
        CLibHbm::func_8045D470(false);
        s1 = (char*)func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x3E);
        func_8022B9B4(&self->mSysWinD4, (u32)s1, 0);
        func_8022BFC8(&self->mSysWinD4, 1);
        func_8022B8B8(&self->mSysWinD4);
        func_80138078(0x7A);
        return;
    }

    if (self->mField129 != 0) {
        s1 = (char*)func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x47);
        func_8022B9B4(&self->mSysWinD4, (u32)s1, 0);
        func_8022BFC8(&self->mSysWinD4, 1);
        func_8022B8B8(&self->mSysWinD4);
        lbl_eu_80662ACC = (u32)(s8)self->mField124;
        self->mField12D = 1;
        func_80138078(0x28);
        return;
    }

    if (self->mField12B != 0) {
        func_8009D018(0x3213, 0);
    }

    CWorkSystem::setSaveLoadInvalidReset(false);
    func_eu_804521BC(1);
    CLibHbm::func_8045D470(false);
    s1 = (char*)func_80136190((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x37);
    func_8022B9B4(&self->mSysWinD4, (u32)s1, 0);
    func_8022BFC8(&self->mSysWinD4, 1);
    func_8022B8B8(&self->mSysWinD4);
    lbl_eu_80662ACC = (u32)(s8)self->mField124;
    func_80138078(0x28);
}
#pragma pop

extern "C" __declspec(noinline) void func_8029078C(CSaveLoad* p) {
    // Function pointer to prevent MWCC from inlining func_8028E964
    int (*checkFn)(CSLCur*) = func_8028E964;
    if (checkFn(&p->mCur) != 0) {
        p->mField121 = 0xb;
        p->mField11C = 3;
        p->mField11E = 0;
        func_80290994(p);
    }
}

// Per-frame update: dispatch on mField121 state, then refresh layout,
// cursor, scrollbar, both syswins and the sub-cursor.
#pragma optimize_for_size on
void func_8028F2CC(CSaveLoad* self) {
    if (self->mField120 == 0) return;

    switch (self->mField121) {
    case 0:
        func_80290094(self);
        break;
    case 1:
        func_802900E0(self);
        break;
    case 2:
        func_80290140(self);
        break;
    case 3:
        func_8029018C(self);
        break;
    case 4:
        func_802901D8(self);
        break;
    case 5:
        func_8029022C(self);
        break;
    case 6:
        // Second window finished: drop back to the selection state
        if (CSysWin_isActive(&self->mSysWinD4) != 0) {
            self->mField121 = 3;
        }
        break;
    case 7:
        func_8029040C(self);
        break;
    case 8:
        func_8029049C(self);
        break;
    case 9:
        func_802904B4(self);
        break;
    case 10:
    case 11:
    case 12:
        func_8029078C(self);
        break;
    }

    ((UnkVtblObj*)self->mLayout)->vf14(0);
    func_8028EAF8((UnkPtrHolder*)((char*)self + 0x28));
    func_801F3540(self->mScrollbar);
    func_8022B748(&self->mSysWin98);
    func_801D202C((char*)self + 0x40);
    func_8022B748(&self->mSysWinD4);
    func_8028EDF8((CSLCur*)((char*)self + 0x110));
}
#pragma optimize_for_size off

int func_8028E964(CSLCur* cur) {
    int result = 0;
    if (cur->mFieldC != 0) {
        if (cur->mFieldD != 0) {
            if (cur->mFieldE != 0) {
                result = 1;
            }
        }
    }
    return result;
}

// Check if a given slot (index 0-2) is valid and has its flag byte set.
// Returns the slot's pointer + 0x20 if valid, 0 otherwise.
extern "C" __declspec(noinline) int func_8028E998(CSLCur* cur, u8 index) {
    if (index >= 3) return 0;

    // Load pointer from the array of 3 pointers at offsets 0, 4, 8
    void* ptr = ((void**)cur)[index];
    if (ptr == nullptr) return 0;

    // Check flag byte at offset 0xF from the indexed entry
    u8* flagAddr = (u8*)cur + index;
    if (flagAddr[0xF] != 0) {
        return (int)((u8*)ptr + 0x20);
    }
    return 0;
}

void func_802907E4(CSaveLoad* p) {
    // Enable animation B, disable animation A
    p->mLayout->SetAnimationEnable(p->mAnimTransB, false);
    p->mLayout->SetAnimationEnable(p->mAnimTransA, true);
}

void func_80290844(CSaveLoad* p) {
    // Enable animation A, disable animation B
    p->mLayout->SetAnimationEnable(p->mAnimTransA, false);
    p->mLayout->SetAnimationEnable(p->mAnimTransB, true);
}

// Refresh cursor/button display state for the save/load screen.
// dont_inline: retail calls this as an out-of-line helper everywhere.
#pragma push
#pragma dont_inline on
#pragma optimize_for_size on
extern "C" void func_802908A4(CSaveLoad* self) {
    if (CSysWin_getUnk34(&self->mSysWin98) != 0) {
        // Window open: refresh the L/R button highlight on the cursor object
        char btnBuf[0xC];
        func_8022C1B4(btnBuf, &self->mSysWin98, self->mField128);
        ((CCur18Obj*)((char*)self + 0x40))->vf4((int)btnBuf);
    } else {
        char* strBase = (char*)&lbl_eu_8050F7CC;
        char numBuf[0x18];
        char paneBuf[0xC];

        // Format the slot number text and set it on the root panes
        sprintf(numBuf, strBase + 0xb7, (s8)self->mField124 + 1);
        void* paneA = ((RootPaneProxy*)*(void**)((char*)self->mLayout + 0x10))->vf15(numBuf, 1);
        void* paneB = ((RootPaneProxy*)*(void**)((char*)self->mLayout + 0x10))->vf15(
            strBase + 0xc4, 1);
        func_80137924(paneBuf, paneA, paneB, *(void**)((char*)self->mLayout + 0x10));
        func_8028EC04((char*)self + 0x28, paneBuf);
    }
}
#pragma optimize_for_size reset
#pragma pop

// Render the save/load screen for all 3 slots.
// Gets localized strings, sets up text elements, buttons, and scrollbar
// for each save slot based on slot data (or empty if no data).
void func_80290994(CSaveLoad* self) {
    char* strBase = (char*)lbl_eu_8050F7CC;

    char* str4a = (char*)func_80136190(&strBase[0xa7], &strBase[0xb2], 0x4a);
    char* str4b = (char*)func_80136190((char*)&strBase[0xa7], (char*)&strBase[0xb2], 0x4b);
    char* str4c = (char*)func_80136190((char*)&strBase[0xa7], (char*)&strBase[0xb2], 0x4c);
    char* str4d = (char*)func_80136190((char*)&strBase[0xa7], (char*)&strBase[0xb2], 0x4d);

    // Retail calls getLanguage separately for each flag.
    u32 isJapanese = (CDeviceSC::getLanguage() == 0);
    u32 isGerman = (CDeviceSC::getLanguage() == 2);

    char slotNameBuf[0x20];
    char textBuf[0x20];
    char emptyBtnBuf[0x20];
    char dataBtnBuf[0x20];

    // Loop through 3 save slots
    for (u8 slot = 0; slot < 3; slot++) {
        u8 slotNum = slot + 1;

        // Format slot name string
        sprintf(slotNameBuf, (const char*)&strBase[0xd3], slotNum);

        // Set up default background text for the slot
        func_80136B4C(self->mLayout, slotNameBuf, str4c, 0u);

        // Check if the slot has save data
        u8* slotData = (u8*)func_8028E998(&self->mCur, slot);
        if (slotData == NULL) {
            // Empty slot
            sprintf(textBuf, (const char*)&strBase[0xde], str4d, str4d);
            sprintf(slotNameBuf, (const char*)&strBase[0xe3], slotNum);
            func_80136A1C(self->mLayout, slotNameBuf, textBuf, 0u);

            sprintf(slotNameBuf, (const char*)&strBase[0xf2], slotNum);
            func_80136B4C(self->mLayout, slotNameBuf, (char*)&strBase[0xfd], 0u);

            sprintf(textBuf, (const char*)&strBase[0xfe], str4d, str4d, str4a, str4d, str4d);
            sprintf(slotNameBuf, (const char*)&strBase[0x109], slotNum);
            func_80136A1C(self->mLayout, slotNameBuf, textBuf, 0u);

            // Language-specific formatting
            if (isJapanese != 0) {
                sprintf(textBuf, (const char*)&strBase[0x117],
                        str4d, str4d, str4d, str4d, str4b, str4d, str4d, str4b, str4d);
            } else if (isGerman != 0) {
                sprintf(textBuf, (const char*)&strBase[0x12c],
                        str4d, str4d, str4d, str4d, str4d, str4d, str4d, str4d, str4d);
            } else {
                sprintf(textBuf, (const char*)&strBase[0x117],
                        str4d, str4d, str4b, str4d, str4d, str4b, str4d, str4d, str4d);
            }

            sprintf(slotNameBuf, (const char*)&strBase[0x13f], slotNum);
            func_80136A1C(self->mLayout, slotNameBuf, textBuf, 0u);

            // Format empty slot detail text
            sprintf(textBuf, (const char*)&strBase[0xfe], str4d, str4d, str4a, str4d, str4d);
            sprintf(slotNameBuf, (const char*)&strBase[0x14c], slotNum);
            func_80136A1C(self->mLayout, slotNameBuf, textBuf, 0u);

            // Hide all 7 per-slot buttons
            for (u8 btn = 0; btn < 7; btn++) {
                sprintf(emptyBtnBuf, (const char*)&strBase[0x15c], btn, btn + 1);
                void* pane =
                    ((RootPaneProxy*)*(void**)((char*)self->mLayout + 0x10))->vf15(emptyBtnBuf, 1);
                if (pane != 0) {
                    func_80124270(pane, 0u);
                }
            }

            // Set up empty slot indicator
            sprintf(slotNameBuf, (const char*)&strBase[0x170], slotNum);
            func_80136B4C(self->mLayout, slotNameBuf, (char*)&strBase[0xfd], 0u);

            // Check for award data
            void* awardData = self->mArcAccessor->GetResource(0x74696D67, (const char*)&strBase[0x17c], 0);
            if (awardData != 0) {
                sprintf(slotNameBuf, (const char*)&strBase[0x18f], slotNum);
                func_80137E7C(self->mLayout, slotNameBuf, awardData);
            }

            // Hide the main slot button
            sprintf(slotNameBuf, (const char*)&strBase[0x19d], slotNum);
            void* pane =
                ((RootPaneProxy*)*(void**)((char*)self->mLayout + 0x10))->vf15(slotNameBuf, 1);
            func_80124270(pane, 0u);
        } else {
            // Slot has save data
            // Format slot data strings
            sprintf(textBuf, (const char*)&strBase[0x1aa], *(u16*)(slotData + 0x64));
            sprintf(slotNameBuf, (const char*)&strBase[0xe3], slotNum);
            func_80136A1C(self->mLayout, slotNameBuf, textBuf, 0u);

            sprintf(slotNameBuf, (const char*)&strBase[0xf2], slotNum);
            func_80136A1C(self->mLayout, slotNameBuf, (char*)(slotData + 0x44), 0u);

            // Format time played string (minutes clamped to 59 when over an hour cap)
            u16 playTime = *(u16*)(slotData + 0xa);
            u16 minutes = *(u16*)(slotData + 0xc);
            if (playTime > 0x63) {
                if (playTime > 0x3e7 || minutes > 0x3b) minutes = 0x3b;
                u16 hours = (playTime > 0x3e7) ? 0x3e7 : playTime;
                sprintf(textBuf, (const char*)&strBase[0x1af], hours, str4a, minutes);
            } else {
                if (minutes > 0x3b) minutes = 0x3b;
                u16 hours = (playTime > 0x63) ? 0x63 : playTime;
                sprintf(textBuf, (const char*)&strBase[0x1ba], hours, str4a, minutes);
            }

            sprintf(slotNameBuf, (const char*)&strBase[0x109], slotNum);
            func_80136A1C(self->mLayout, slotNameBuf, textBuf, 0u);

            // Format slot details based on language and save data
            if (isJapanese != 0) {
                sprintf(textBuf, (const char*)&strBase[0x1c5],
                        *(u16*)(slotData + 0x4), str4b,
                        *(u16*)(slotData + 0x6), str4b,
                        *(u8*)(slotData + 0x9));
            } else if (isGerman != 0) {
                sprintf(textBuf, (const char*)&strBase[0x1d6],
                        *(u8*)(slotData + 0x9),
                        *(u16*)(slotData + 0x6),
                        *(u16*)(slotData + 0x4));
            } else {
                sprintf(textBuf, (const char*)&strBase[0x1e5],
                        *(u8*)(slotData + 0x9), str4b,
                        *(u16*)(slotData + 0x6), str4b,
                        *(u16*)(slotData + 0x4));
            }

            sprintf(slotNameBuf, (const char*)&strBase[0x13f], slotNum);
            func_80136A1C(self->mLayout, slotNameBuf, textBuf, 0u);

            sprintf(textBuf, (const char*)&strBase[0x1ba],
                    *(u8*)(slotData + 0x8), str4a, *(u8*)(slotData + 0x2));
            sprintf(slotNameBuf, (const char*)&strBase[0x14c], slotNum);
            func_80136A1C(self->mLayout, slotNameBuf, textBuf, 0u);

            // Set up the 7 award buttons
            for (u8 btn = 0; btn < 7; btn++) {
                sprintf(dataBtnBuf, (const char*)&strBase[0x15c], btn, btn + 1);
                void* pane =
                    ((RootPaneProxy*)*(void**)((char*)self->mLayout + 0x10))->vf15(dataBtnBuf, 1);

                // Record presence flag lives in the low byte of a word entry
                u32 rec = *(u32*)(slotData + btn * 4 + 0x14);
                if ((u8)rec != 0) {
                    if (pane != 0) {
                        func_80124270(pane, 1u);
                    }
                    // Look up the award icon texture and attach it
                    char* iconName = (char*)func_80138F78(
                        (u16)func_80136254(lbl_eu_80664090, (const char*)&strBase[0x1f6], (u8)rec));
                    void* awardRes =
                        ((nw4r::lyt::ArcResourceAccessor*)func_801355F4())
                            ->GetResource(0x74696D67, iconName, 0);
                    if (awardRes != 0) {
                        sprintf(textBuf, (const char*)&strBase[0x1ff], slotNum, btn + 1);
                        ((RootPaneProxy*)*(void**)((char*)self->mLayout + 0x10))->vf15(textBuf, 1);
                        func_80137E7C(self->mLayout, textBuf, awardRes);
                    }
                } else {
                    if (pane != 0) {
                        func_80124270(pane, 0u);
                    }
                }
            }

            // Resolve the character-class indicator string
            u8 charResult = func_80141BA0(*(u16*)(slotData + 0xe), *(u8*)(slotData + 0x11));
            char* slotIndicator;
            // NOTE: retail reloads the +0x66 class byte for each test (no cached local).
            if (*(u8*)(slotData + 0x66) != 0) {
                slotIndicator = (char*)func_80136190((char*)&strBase[0xa7], (char*)&strBase[0xb2], 0x53);
            } else {
                slotIndicator = func_8013639C(lbl_eu_806640A8, &strBase[0xb2], (u16)charResult);
            }

            if (*(u8*)(slotData + 0x67) != 0 && charResult == 1) {
                slotIndicator = (char*)func_80136190((char*)&strBase[0xa7], (char*)&strBase[0xb2], 0x30);
            }

            sprintf(slotNameBuf, (const char*)&strBase[0x170], slotNum);
            func_80136B4C(self->mLayout, slotNameBuf, slotIndicator, 0u);

            // Copy slot data to the heap buffer and flush it for GX
            memcpy(*(void**)((u8*)self + 0x130 + slot * 4), (const void*)(slotData + 0x80), 0x9C00);
            DCStoreRange(*(void**)((u8*)self + 0x130 + slot * 4), 0x9C00);

            // Handle class-specific data
            void* classData;
            if (*(u8*)(slotData + 0x66) != 0) {
                classData = self->mArcAccessor->GetResource(0x74696D67, (const char*)&strBase[0x211], 0);
            } else {
                classData = *(void**)((u8*)self + 0x130 + slot * 4);
            }

            if (classData != 0) {
                sprintf(slotNameBuf, (const char*)&strBase[0x18f], slotNum);
                func_80137E7C(self->mLayout, slotNameBuf, classData);
            }

            // Show/hide the final button depending on the character's sex flag
            sprintf(slotNameBuf, (const char*)&strBase[0x19d], slotNum);
            void* pane =
                ((RootPaneProxy*)*(void**)((char*)self->mLayout + 0x10))->vf15(slotNameBuf, 1);
            func_80124270(pane, (*(u8*)(slotData + 0x67) != 0) ? 1u : 0u);
        }
    }
}

// Find the best save slot by comparing fields in priority order.
// If global value >= 0, use it directly. Otherwise iterate slots 0-2
// and find the one with the highest field values (lexicographic comparison).
#pragma optimize_for_size on
extern "C" void func_802910D4(CSaveLoad* self) {
    // Cast to function pointer to prevent MWCC from inlining func_8028E998
    typedef u8* (*GetSlotFn)(CSLCur*, u8);

    s32 globalVal = (s32)lbl_eu_80662ACC;
    if (globalVal >= 0) {
        self->mField124 = (u8)globalVal;
        return;
    }

    s32 best = -1;
    u32 i;
    for (i = 0; i < 3; i++) {
        u8* slot = ((GetSlotFn)func_8028E998)(&self->mCur, (u8)i);
        if (slot == nullptr) continue;

        // Compare fields in priority order: +4, +6, +9, +8, +2, +3
        if (best >= 0) {
            u8* bestSlot = ((GetSlotFn)func_8028E998)(&self->mCur, (u8)best);
            if (*(u16*)(bestSlot + 4) < *(u16*)(slot + 4)) {
                best = i & 0xFF;
            } else if (*(u16*)(bestSlot + 4) == *(u16*)(slot + 4)) {
                if (*(u16*)(bestSlot + 6) < *(u16*)(slot + 6)) {
                    best = i & 0xFF;
                } else if (*(u16*)(bestSlot + 6) == *(u16*)(slot + 6)) {
                    if (bestSlot[9] < slot[9]) {
                        best = i & 0xFF;
                    } else if (bestSlot[9] == slot[9]) {
                        if (bestSlot[8] < slot[8]) {
                            best = i & 0xFF;
                        } else if (bestSlot[8] == slot[8]) {
                            if (bestSlot[2] < slot[2]) {
                                best = i & 0xFF;
                                continue;
                            }
                            // Retail reuses the cr0 result here (bge) rather than
                            // testing equality, leaving the +3 check unreachable.
                            if (bestSlot[2] >= slot[2]) continue;
                            if (bestSlot[3] < slot[3]) {
                                best = i & 0xFF;
                            }
                        }
                    }
                }
            }
        } else {
            best = i & 0xFF;
        }
    }

    if (best != -1) {
        self->mField124 = (u8)best;
    }
}
#pragma optimize_for_size off

// Load global CSaveLoad* from lbl_eu_806649F4. Takes 4 parameters (r3-r6)
// but only uses r5 (flag: non-zero = handle mField12C/mField129 branches + play sound)
// and r6 (byte value stored into mField12F when r5 == 0).
// r3 and r4 are unused params - they exist so r5/r6 are in the correct register slots.
// No local variable for the global pointer - access lbl_eu_806649F4 directly
// to force MWCC to reload after each function call, matching retail's repeated @sda21 loads.
void func_80291204(int, int, int r5, int r6) {
    if (lbl_eu_806649F4 == nullptr) return;

    if (r5 != 0) {
        if (lbl_eu_806649F4->mField12C != 0) {
            lbl_eu_806649F4->mField121 = 0xc;
            // Cast call prevents MWCC from inlining func_8028E8EC's body
            // (same TU, -inline auto) while keeping a direct bl call.
            ((void (*)(CSLCur*))func_8028E8EC)(&lbl_eu_806649F4->mCur);
        } else if (lbl_eu_806649F4->mField129 != 0) {
            lbl_eu_806649F4->mField121 = 0xb;
            // Local pointer so MWCC reuses it for both stores (retail keeps
            // the reloaded pointer in one register for mField11C/mField11E).
            CSaveLoad* q = lbl_eu_806649F4;
            q->mField11C = 3;
            q->mField11E = 0;
        } else {
            lbl_eu_806649F4->mField121 = 0xc;
            ((void (*)(CSLCur*))func_8028E8EC)(&lbl_eu_806649F4->mCur);
        }
        code80135FDC_thunk_BFE8C((u8*)0x80);
    } else {
        lbl_eu_806649F4->mField121 = 0xb;
        // Local pointer so MWCC reuses it for the mField11C/mField11E pair
        // (retail keeps one register for both stores).
        CSaveLoad* q = lbl_eu_806649F4;
        q->mField11C = 3;
        q->mField11E = 0;
        lbl_eu_806649F4->mField12E = 1;
        lbl_eu_806649F4->mField12F = (u8)r6;
    }
}

// Despite Fv mangling, receives (self, event) with event in r4.
// Checks if the event's file handle matches self->mFileHandle before proceeding
// with layout initialization, text setup, and cursor state initialization.
// Returns 1 on success, 0 on failure (file handle mismatch).
// optimize_for_size gives the retail stmw r28 / lmw r28 save-restore pair.
#pragma push
#pragma optimize_for_size on
int OnFileEvent__9CSaveLoadFv(CSaveLoad* self, CEventFile* event) {
    // if-equal form reproduces retail's fallthrough-body / tail return-0 shape
    if (self->mFileHandle == event->mFileHandle) {
    // handle fetched first, consumed immediately (dies after createRegion)
    int regionHandle = (int)getHandleMEM2__Q23mtl10MemManagerFv();
    u8* strBase = lbl_eu_8050F7CC;

    // Create the resource region for the base class at offset 4
    ((UnkClass_8045F564*)((u8*)self + 4))->createRegion(
        regionHandle, 0x10000, (const char*)&strBase[0x224], 0);

    // RAII scratch-region guard plus stack temporaries, declared in retail
    // frame-slot order (guard sp+0x08, sub-cursor out sp+0x10, cursor-reset
    // out sp+0x20, CCur18 temp sp+0x38, CSLCur temp sp+0x50).
    // Distinct pointer-expression spellings defeat MWCC CSE of (self+4),
    // which otherwise pins an extra nonvolatile register.
    Class_8045F858 regionGuard(reinterpret_cast<UnkClass_8045F564*>(&self->_pad_04[0]));
    u8 subCurBuf[0x10];
    u8 curResetBuf[0x18];
    u8 cur18Tmp[0x18];
    u8 slCurBuf[0x18];

    // Detach the payload pointer from the file handle (cleared to null)
    FileHandleView* fh = (FileHandleView*)self->mFileHandle;
    void* fileData = fh->mData;
    fh->mData = nullptr;
    mtl::MemManager::func_80434A4C(false);

    // Fresh arc resource accessor bound to the detached arc payload
    // (local keeps the value in r3 for the Attach call, matching retail)
    nw4r::lyt::ArcResourceAccessor* accessor = createArcResourceAccessor__10CLibLayoutFv();
    self->mArcAccessor = accessor;
    Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(
        accessor, fileData, (const char*)&strBase[0x22e]);

    // Create layout and both animation transforms
    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        &self->mLayout, self->mArcAccessor, (const char*)&strBase[0x232]);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        self->mLayout, &self->mAnimTransA, self->mArcAccessor, (char*)&strBase[0x248]);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        self->mLayout, &self->mAnimTransB, self->mArcAccessor, (char*)&strBase[0x261]);

    // Font setup: root pane -> font object -> apply font to pane
    void* rootPane = *(void**)((char*)self->mLayout + 0x10);
    void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, self->mLayout);
    u32 fontResult = static_cast<FontHelper*>(fontObj)->v7();
    func_8013676C((nw4r::lyt::Pane*)rootPane, fontResult);

    // Japanese-only caption strings applied to fixed pane names
    char* capJp = (char*)func_801355D8();
    if (capJp != nullptr) {
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x27f], (u32)capJp);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x288], (u32)capJp);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x295], (u32)capJp);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x29e], (u32)capJp);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x2ab], (u32)capJp);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x2b4], (u32)capJp);
    }

    // Localized caption strings applied to fifteen fixed pane names
    char* capLoc = (char*)func_801355BC();
    if (capLoc != nullptr) {
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x2c1], (u32)capLoc);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x2cd], (u32)capLoc);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x2d9], (u32)capLoc);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x2e5], (u32)capLoc);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x2f0], (u32)capLoc);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x2fe], (u32)capLoc);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x30a], (u32)capLoc);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x316], (u32)capLoc);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x322], (u32)capLoc);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x32d], (u32)capLoc);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x33b], (u32)capLoc);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x347], (u32)capLoc);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x353], (u32)capLoc);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x35f], (u32)capLoc);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(self->mLayout, (char*)&strBase[0x36a], (u32)capLoc);
    }

    // Function-pointer casts keep MWCC from inlining these same-TU helpers
    // (retail calls them out-of-line).
    ((void (*)(CSaveLoad*))func_802907E4)(self);
    ((UnkVtblObj*)self->mLayout)->vf14(0);

    // L/R button labels for all three rows
    char* str31 = (char*)func_80136190((char*)&strBase[0xa7], (char*)&strBase[0xb2], 0x31);
    char* str32 = (char*)func_80136190((char*)&strBase[0xa7], (char*)&strBase[0xb2], 0x32);

    // Set up text on layout with the button labels
    func_80136B4C(self->mLayout, (char*)&strBase[0x378], str32, 0u);
    func_80136B4C(self->mLayout, (char*)&strBase[0x389], str31, 0u);
    func_80136B4C(self->mLayout, (char*)&strBase[0x39a], str32, 0u);
    func_80136B4C(self->mLayout, (char*)&strBase[0x3ab], str31, 0u);
    func_80136B4C(self->mLayout, (char*)&strBase[0x3bc], str32, 0u);
    func_80136B4C(self->mLayout, (char*)&strBase[0x3cd], str31, 0u);

    // Hide three named panes via the root pane lookup virtual
    void* pane = ((RootPaneProxy*)*(void**)((char*)self->mLayout + 0x10))->vf15((const char*)&strBase[0x3de], 1);
    func_80124270(pane, 0);
    pane = ((RootPaneProxy*)*(void**)((char*)self->mLayout + 0x10))->vf15((const char*)&strBase[0x3ea], 1);
    func_80124270(pane, 0);
    pane = ((RootPaneProxy*)*(void**)((char*)self->mLayout + 0x10))->vf15((const char*)&strBase[0x3f6], 1);
    func_80124270(pane, 0);

    // Build a stack CSLCur bound to the arc accessor and spill its fields into
    // the embedded cursor object at self+0x28, then run its slot-8 virtual.
    __ct__CSLCur((CSLCur*)slCurBuf, (int)self->mArcAccessor);
    CurMirror28* src28 = reinterpret_cast<CurMirror28*>(slCurBuf);
    CurMirror28* dst28 = reinterpret_cast<CurMirror28*>((u8*)self + 0x28);
    dst28->w0 = src28->w0;
    dst28->w4 = src28->w4;
    dst28->w8 = src28->w8;
    dst28->wC = src28->wC;
    dst28->b14 = src28->b14;
    dst28->b15 = src28->b15;
    ((VtSlot8Call*)((u8*)self + 0x28))->vf2();

    // Same pattern for the CCur18 sub-object at self+0x40
    __ct__CCur18(cur18Tmp, func_801355F4());
    CurMirror40* src40 = reinterpret_cast<CurMirror40*>(cur18Tmp);
    CurMirror40* dst40 = reinterpret_cast<CurMirror40*>((u8*)self + 0x40);
    dst40->w4 = src40->w4;
    dst40->w8 = src40->w8;
    dst40->wC = src40->wC;
    dst40->w10 = src40->w10;
    dst40->b14 = src40->b14;
    dst40->b15 = src40->b15;
    __dt__6CCur18Fv(cur18Tmp, -1);
    ((VtSlot8Call*)((u8*)self + 0x40))->vf2();

    // Sub-layout cursor built on the stack, then spilled to self+0x110
    ((void (*)(CSLCur*, int))func_8028ED0C)(
        reinterpret_cast<CSLCur*>(subCurBuf), (int)self->mArcAccessor);
    {
        CurMirror110* src110 = reinterpret_cast<CurMirror110*>(subCurBuf);
        CurMirror110* dst110 = reinterpret_cast<CurMirror110*>((u8*)self + 0x110);
        dst110->w0 = src110->w0;
        dst110->w4 = src110->w4;
        dst110->w8 = src110->w8;
        dst110->bC = src110->bC;
        dst110->bD = src110->bD;
        dst110->bE = src110->bE;
    }

    ((void (*)(UnkED70_Struct*))func_8028ED70)(
        reinterpret_cast<UnkED70_Struct*>((u8*)self + 0x110));

    // Reset the main cursor via a stack temporary, then spill it to self+0x13C.
    // The copy uses typed members (u16/u8) so MWCC reproduces retail's
    // halfword/byte store widths exactly.
    ((void (*)(CSLCur*))func_8028E7C8)(reinterpret_cast<CSLCur*>(curResetBuf));
    CurMirror13C* src13C = reinterpret_cast<CurMirror13C*>(curResetBuf);
    CurMirror13C* dst13C = reinterpret_cast<CurMirror13C*>(&self->mCur);
    dst13C->w0 = src13C->w0;
    dst13C->w4 = src13C->w4;
    dst13C->w8 = src13C->w8;
    dst13C->hwC = src13C->hwC;
    dst13C->bE = src13C->bE;
    dst13C->hwF = src13C->hwF;
    dst13C->b11 = src13C->b11;

    // Allocate the three save-data buffers and register their callbacks
    ((void (*)(UnkStruct_3Ptr*))func_8028E838)(
        reinterpret_cast<UnkStruct_3Ptr*>(&self->mCur));
    ((void (*)(CSLCur*))func_8028E8EC)(reinterpret_cast<CSLCur*>(&self->mCur));

    // Set flags
    self->mField122 = 1;
    self->mField120 = 1;
    self->mFileHandle = 0;
    ((UnkClass_8045F564*)((char*)self + 0x04))->func_8045F810();

    // Allocate three 0x9C00-byte buffers from MEM2 (size written as
    // 0x10000-0x6400 to reproduce retail's lis/subi pair)
    self->mField130 = allocate_head__Q23mtl10MemManagerFUlUli((u32)getHandleMEM2__Q23mtl10MemManagerFv(), 0x10000 - 0x6400, 0x20);
    self->mField134 = allocate_head__Q23mtl10MemManagerFUlUli((u32)getHandleMEM2__Q23mtl10MemManagerFv(), 0x10000 - 0x6400, 0x20);
    self->mField138 = allocate_head__Q23mtl10MemManagerFUlUli((u32)getHandleMEM2__Q23mtl10MemManagerFv(), 0x10000 - 0x6400, 0x20);

    // regionGuard destructor runs here (retail __dt__14Class_8045F858Fv)
    return 1;
    }
    return 0;
}
#pragma pop

u32 func_8029183C(void) {
    extern u32 lbl_eu_80662AD0;
    return lbl_eu_80662AD0;
}

cf::CfAward::~CfAward() {
    // Restore vtable to the complete object vtable before destruction
    void* base = (void*)&lbl_eu_80538858;
    mSecondBase = base;
    mVtbl = (char*)base + 8;

    // Call the cleanup function (subobject destructor)
    func_8009D514(this);
}

// Count valid entries (0-299) and award achievements based on thresholds.
// Returns immediately if the initial parameter check fails.
void func_802918AC(int val) {
    int i;
    int count;

    // Check if the input parameter is valid first
    if (((int (*)(int))func_80291C60)(val) == 0) return;

    count = 0;
    i = 0;
    while (i < 300) {
        if (((int (*)(int))func_80291C60)(i) != 0) {
            if (((u32 (*)(u32))func_8009CF8C)(i + 0x1d44) != 0) {
                count++;
            }
        }
        i++;
    }

    // Award achievements based on number of completed entries
    if ((u32)count >= 1) {
        func_8027EEF4(8);
    }
    if ((u32)count >= 10) {
        func_8027EEF4(9);
    }
    if ((u32)count >= 100) {
        func_8027EEF4(10);
    }
}

extern "C" void func_8029194C() {
    u32 result = func_8027EE88(0x8f, 1);
    if (result >= 0x64) {
        func_8027EEF4(0x8f);
    }
    if (result >= 0x3e8) {
        func_8027EEF4(0x90);
    }
}

extern "C" void func_802919A0() {
    u32 result = func_8027EE88(0x91, 1);
    if (result >= 1) {
        func_8027EEF4(0x91);
    }
    if (result >= 0x32) {
        func_8027EEF4(0x92);
    }
    if (result >= 0x12c) {
        func_8027EEF4(0x93);
    }
}

// Forward declarations for retail functions not yet decompiled in this TU
// Check if a set of specific flags are all set.
// Returns 1 if all flags are valid (non-zero), 0 otherwise.
// Uses bool conversion to match MWCC's neg+or+srwi. pattern.
extern "C" u32 func_80291D98() {
    bool b;
    b = func_8009CF8C(0xa26);
    if (!b) return 0;
    b = func_8009CF8C(0xa2c);
    if (!b) return 0;
    b = func_8009CF8C(0xa2d);
    if (!b) return 0;
    b = func_8009CF8C(0xa2e);
    if (!b) return 0;
    b = func_8009CF8C(0xa2f);
    if (!b) return 0;
    b = func_8009CF8C(0xa30);
    if (!b) return 0;
    b = func_8009CF8C(0xa32);
    if (!b) return 0;
    b = func_8009CF8C(0xa34);
    if (!b) return 0;
    b = func_8009CF8C(0xa4b);
    if (b) goto ret1;
    b = func_8009CF8C(0xac5);
    if (b) goto ret1;
    return 0;

ret1:
    return 1;
}

// Check if flags 0xad6/0xb48, 0xad7/0xb47, 0xb38-0xb41, 0xb44-0xb46 are all set.
// Returns 1 if all pass, 0 otherwise.
extern "C" u32 func_80292EC0() {
    bool b = func_8009CF8C(0xad6);
    if (b) {} else {
        b = func_8009CF8C(0xb48);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xad7);
    if (b) {} else {
        b = func_8009CF8C(0xb47);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xb38);
    if (b) {} else return 0;
    b = func_8009CF8C(0xb39);
    if (b) {} else return 0;
    b = func_8009CF8C(0xb3a);
    if (b) {} else return 0;
    b = func_8009CF8C(0xb3b);
    if (b) {} else return 0;
    b = func_8009CF8C(0xb3c);
    if (b) {} else return 0;
    b = func_8009CF8C(0xb3d);
    if (b) {} else return 0;
    b = func_8009CF8C(0xb3e);
    if (b) {} else return 0;
    b = func_8009CF8C(0xb3f);
    if (b) {} else return 0;
    b = func_8009CF8C(0xb40);
    if (b) {} else return 0;
    b = func_8009CF8C(0xb41);
    if (b) {} else return 0;
    b = func_8009CF8C(0xb44);
    if (b) {} else return 0;
    b = func_8009CF8C(0xb45);
    if (b) {} else return 0;
    b = func_8009CF8C(0xb46);
    return b;
}

// Check if a comprehensive set of flags (0xa85-0xab1) are all set.
// Some checks have fallback flags if the primary is not set.
// Returns 1 if all pass, 0 otherwise.
u32 func_80292418() {
    bool b = func_8009CF8C(0xa85);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa86);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa87);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa88);
    if (b) {} else {
        b = func_8009CF8C(0xad8);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xa89);
    if (b) {} else {
        b = func_8009CF8C(0xac9);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xa8a);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa8b);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa8c);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa8d);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa8e);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa8f);
    if (b) {} else {
        b = func_8009CF8C(0xa7e);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xa90);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa91);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa92);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa93);
    if (b) {} else {
        b = func_8009CF8C(0xacc);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xa94);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa95);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa96);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa97);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa98);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa99);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa9a);
    if (b) {} else {
        b = func_8009CF8C(0xace);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xa9b);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa9c);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa9d);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa9e);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa9f);
    if (b) {} else return 0;
    b = func_8009CF8C(0xaa0);
    if (b) {} else return 0;
    b = func_8009CF8C(0xaa1);
    if (b) {} else {
        b = func_8009CF8C(0xacd);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xaa2);
    if (b) {} else return 0;
    b = func_8009CF8C(0xaa3);
    if (b) {} else return 0;
    b = func_8009CF8C(0xaa4);
    if (b) {} else {
        b = func_8009CF8C(0xa80);
        if (b) {} else {
            b = func_8009CF8C(0xa81);
            if (b) {} else return 0;
        }
    }
    b = func_8009CF8C(0xaa5);
    if (b) {} else return 0;
    b = func_8009CF8C(0xaa6);
    if (b) {} else return 0;
    b = func_8009CF8C(0xaa7);
    if (b) {} else {
        b = func_8009CF8C(0xaca);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xaa8);
    if (b) {} else {
        b = func_8009CF8C(0xac8);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xab1);
    if (b) {} else return 0;
    return 1;
}

// Check if flags 0xa71-0xa79, 0xa70, 0xac1-0xac5, 0xac6/0xac7, 0xac8-0xacc,
// 0xacd/0xace, 0xacf-0xad3, 0xad4/0xad5, 0xad6, 0xad7 are all set.
// Returns 1 if all pass, 0 otherwise.
extern "C" u32 func_80292000() {
    bool b = func_8009CF8C(0xa71);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa72);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa73);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa74);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa75);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa76);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa77);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa78);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa79);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa70);
    if (b) {} else return 0;
    b = func_8009CF8C(0xac1);
    if (b) {} else return 0;
    b = func_8009CF8C(0xac2);
    if (b) {} else return 0;
    b = func_8009CF8C(0xac3);
    if (b) {} else return 0;
    b = func_8009CF8C(0xac4);
    if (b) {} else return 0;
    b = func_8009CF8C(0xac5);
    if (b) {} else return 0;
    b = func_8009CF8C(0xac6);
    if (b) {} else {
        b = func_8009CF8C(0xac7);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xac8);
    if (b) {} else return 0;
    b = func_8009CF8C(0xac9);
    if (b) {} else return 0;
    b = func_8009CF8C(0xaca);
    if (b) {} else return 0;
    b = func_8009CF8C(0xacb);
    if (b) {} else return 0;
    b = func_8009CF8C(0xacc);
    if (b) {} else return 0;
    b = func_8009CF8C(0xacd);
    if (b) {} else {
        b = func_8009CF8C(0xace);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xacf);
    if (b) {} else return 0;
    b = func_8009CF8C(0xad0);
    if (b) {} else return 0;
    b = func_8009CF8C(0xad1);
    if (b) {} else return 0;
    b = func_8009CF8C(0xad2);
    if (b) {} else return 0;
    b = func_8009CF8C(0xad3);
    if (b) {} else return 0;
    b = func_8009CF8C(0xad4);
    if (b) {} else {
        b = func_8009CF8C(0xad5);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xad6);
    if (b) {} else return 0;
    b = func_8009CF8C(0xad7);
    return b;
}

// Check if various flag groups (0xa66, 0xa67, 0xaa9/0xb05, 0xaaa/0xb07,
// 0xaab/0xad0/0xb1f, 0xacf/0xae7/0xb1b, 0xad1/0xae8/0xb1c, 0xad2/0xaea/0xb1e,
// 0xad3/0xae9/0xb1d, 0xad4/0xaeb/0xb20, 0xad5/0xaec/0xb21, 0xaed/0xb23,
// 0xadf/0xafe, 0xae0/0xb00, 0xae1/0xb04, 0xae2/0xb0d, 0xae3/0xb11,
// 0xae4/0xb13, 0xae5/0xb14, 0xae6/0xb18) are set.
// Returns 1 if all pass, 0 otherwise.
extern "C" u32 func_802929C8() {
    bool b = func_8009CF8C(0xa66);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa67);
    if (b) {} else return 0;
    b = func_8009CF8C(0xaa9);
    if (b) {} else {
        b = func_8009CF8C(0xb05);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xaaa);
    if (b) {} else {
        b = func_8009CF8C(0xb07);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xaab);
    if (b) {} else {
        b = func_8009CF8C(0xad0);
        if (b) {} else {
            b = func_8009CF8C(0xb1f);
            if (b) {} else return 0;
        }
    }
    b = func_8009CF8C(0xacf);
    if (b) {} else {
        b = func_8009CF8C(0xae7);
        if (b) {} else {
            b = func_8009CF8C(0xb1b);
            if (b) {} else return 0;
        }
    }
    b = func_8009CF8C(0xad1);
    if (b) {} else {
        b = func_8009CF8C(0xae8);
        if (b) {} else {
            b = func_8009CF8C(0xb1c);
            if (b) {} else return 0;
        }
    }
    b = func_8009CF8C(0xad2);
    if (b) {} else {
        b = func_8009CF8C(0xaea);
        if (b) {} else {
            b = func_8009CF8C(0xb1e);
            if (b) {} else return 0;
        }
    }
    b = func_8009CF8C(0xad3);
    if (b) {} else {
        b = func_8009CF8C(0xae9);
        if (b) {} else {
            b = func_8009CF8C(0xb1d);
            if (b) {} else return 0;
        }
    }
    b = func_8009CF8C(0xad4);
    if (b) {} else {
        b = func_8009CF8C(0xaeb);
        if (b) {} else {
            b = func_8009CF8C(0xb20);
            if (b) {} else return 0;
        }
    }
    b = func_8009CF8C(0xad5);
    if (b) {} else {
        b = func_8009CF8C(0xaec);
        if (b) {} else {
            b = func_8009CF8C(0xb21);
            if (b) {} else return 0;
        }
    }
    b = func_8009CF8C(0xaed);
    if (b) {} else {
        b = func_8009CF8C(0xb23);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xadf);
    if (b) {} else {
        b = func_8009CF8C(0xafe);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xae0);
    if (b) {} else {
        b = func_8009CF8C(0xb00);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xae1);
    if (b) {} else {
        b = func_8009CF8C(0xb04);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xae2);
    if (b) {} else {
        b = func_8009CF8C(0xb0d);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xae3);
    if (b) {} else {
        b = func_8009CF8C(0xb11);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xae4);
    if (b) {} else {
        b = func_8009CF8C(0xb13);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xae5);
    if (b) {} else {
        b = func_8009CF8C(0xb14);
        if (b) {} else return 0;
    }
    b = func_8009CF8C(0xae6);
    if (b) {} else {
        b = func_8009CF8C(0xb18);
        if (b) {} else return 0;
    }
    return 1;
}

u32 func_80291EF0();

// Count valid save entries (0-299) and award achievements based on thresholds.
// Also checks other game completion milestones.
void func_80291A04() {
    int i;
    u32 count = 0;
    for (i = 0; i < 300; i++) {
        bool b = func_8009CF8C(i + 0xa20);
        if (b) count++;
    }
    
    if (count >= 1) func_8027EEF4(0x94);
    if (count >= 10) func_8027EEF4(0x95);
    if (count >= 50) func_8027EEF4(0x96);
    if (count >= 120) func_8027EEF4(0x97);
    
    if (func_80291D98()) func_8027EEF4(0x98);
    if (func_80291EF0()) func_8027EEF4(0x99);
    if (func_80292000()) func_8027EEF4(0x9a);
    if (func_80292418()) func_8027EEF4(0x9b);
    if (func_802929C8()) func_8027EEF4(0x9c);
    if (func_80292EC0()) func_8027EEF4(0x9d);
}



// Despite Fv mangling, receives (self, val) with val in r4 -
// the value is checked against the [0x28, 0x3e) window.
// Check all pairs of values (1-7) and award achievements based on results.
// allFlag is set if all pairs have result >= 2000.
// allFlag2 is set if all pairs have result >= 5000.
extern "C" void func_80291B30() {
    int allFlag = 1;
    int allFlag2 = 1;
    int i = 1;
    int j;

    for (; i <= 7; i++) {
        for (j = i + 1; j <= 7; j++) {
            int result = func_800824FC__Q22cf13CfGameManagerFv(i, j);

            if (result >= 1000) {
                func_8027EEF4(0xaf);
            }
            if (result >= 3000) {
                func_8027EEF4(0xb0);
            }
            if (result >= 5000) {
                func_8027EEF4(0xb1);
            }
            if (result < 2000) {
                allFlag = 0;
            }
            if (result < 5000) {
                allFlag2 = 0;
            }
        }
    }

    if (allFlag != 0) {
        func_8027EEF4(0xb2);
    }
    if (allFlag2 != 0) {
        func_8027EEF4(0xb3);
    }
}

// Despite Fv mangling, receives (self, val) with val in r4 -
// the value is checked against the [0x28, 0x3e) window.
extern "C" void func_80291B18__Q22cf7CfAwardFv(cf::CfAward*, int val) {
    if (val < 0x28) return;
    if (val >= 0x3e) return;
    func_80291B30();
}


// Count valid items/quests: iterates IDs 0-299, checks validity via
// func_80291C60 and func_8009CF8C, returns the count of valid entries
// Retail symbol is unmangled; declared extern "C" in CfGameManager.hpp
extern "C" s32 func_80291BF8() {
    int count;
    int i;
    count = 0;
    i = 0;
    while (i < 300) {
        if (((int (*)(int))func_80291C60)(i) != 0) {
            if (((u32 (*)(u32))func_8009CF8C)(i + 0x1d44) != 0) {
                count++;
            }
        }
        i++;
    }
    return count;
}

// Check if a value is within the valid range.
// Returns 0 for specific values that are excluded, 1 otherwise.
// Binary-search dispatch tree; transcribed 1:1 from retail.
extern "C" int func_80291C60(int v) {
    if (v == 0x8B)
        goto L_8029434C;
    if (v >= 0x8B)
        goto L_802942C4;
    if (v == 0x13)
        goto L_8029432C;
    if (v >= 0x13)
        goto L_802942A0;
    if (v == 0x10)
        goto L_80294314;
    if (v >= 0x10)
        goto L_80294294;
    if (v >= 0xF)
        goto L_8029430C;
    if (v >= 0x0)
        goto L_8029438C;
    goto L_80294304;

L_80294294:
    if (v >= 0x12)
        goto L_80294324;
    goto L_8029431C;

L_802942A0:
    if (v == 0x6E)
        goto L_8029433C;
    if (v >= 0x6E)
        goto L_802942B8;
    if (v >= 0x6D)
        goto L_80294334;
    goto L_8029438C;

L_802942B8:
    if (v >= 0x70)
        goto L_8029438C;
    goto L_80294344;

L_802942C4:
    if (v == 0xE5)
        goto L_80294374;
    if (v >= 0xE5)
        goto L_802942EC;
    if (v == 0xE3)
        goto L_80294364;
    if (v >= 0xE3)
        goto L_8029436C;
    if (v == 0x8D)
        goto L_8029435C;
    if (v >= 0x8D)
        goto L_8029438C;
    goto L_80294354;

L_802942EC:
    if (v == 0xE7)
        goto L_80294384;
    if (v < 0xE7)
        goto L_8029437C;
    if (v >= 0x12C)
        goto L_80294304;
    goto L_8029438C;

L_80294304:
    return 0;
L_8029430C:
    return 0;
L_80294314:
    return 0;
L_8029431C:
    return 0;
L_80294324:
    return 0;
L_8029432C:
    return 0;
L_80294334:
    return 0;
L_8029433C:
    return 0;
L_80294344:
    return 0;
L_8029434C:
    return 0;
L_80294354:
    return 0;
L_8029435C:
    return 0;
L_80294364:
    return 0;
L_8029436C:
    return 0;
L_80294374:
    return 0;
L_8029437C:
    return 0;
L_80294384:
    return 0;
L_8029438C:
    return 1;
}

// Check if all save/load related resources are available
u32 func_80291EF0() {
    bool b = func_8009CF8C(0xa61);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa7a);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa60);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa62);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa63);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa64);
    if (b) {} else return 0;
    b = func_8009CF8C(0xa65);
    if (b) {} else return 0;
    b = func_8009CF8C(0xaee);
    return b;
}



// sinit: static initializer for the global CfAward instance at lbl_eu_806649F8.
// Sets the manually-managed vtable pointers, runs the constructor body, and
// registers the atexit destructor.
extern "C" void sinit_802930E0() {
    lbl_eu_806649F8.mSecondBase = (void*)&lbl_eu_80538858;
    lbl_eu_806649F8.mVtbl = (char*)&lbl_eu_80538858 + 8;
    func_8009D414(&lbl_eu_806649F8);
    __register_global_object(&lbl_eu_806649F8, (void*)__dt__Q22cf7CfAwardFv, (void*)lbl_eu_80576CF8);
}

// Virtual dispatch through self->mPtr using proper typed virtual calls
// Use local variables for the pointer and vtable to match retail's register allocation
// (self saved to r31, mPtr loaded into r3 each time)
// noinline: retail keeps this out-of-line (called from CSLCur::func_8028EA74 etc.)
extern "C" __declspec(noinline) void func_8028EC74(UnkPtrHolder* self) {
    self->mPtr->vf8(self->mField10);
    self->mPtr->vf7(self->mField0C);
    self->mPtr->vf11(self->mField0C, 1);
    *(float*)((char*)self->mField0C + 0x10) = lbl_eu_80668B6C;
    self->mPtr->vf14(0);
}
