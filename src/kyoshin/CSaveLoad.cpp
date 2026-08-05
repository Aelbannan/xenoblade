// Auto-scaffolded catalog TU for kyoshin/CSaveLoad
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CSaveLoad.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkSystem.hpp"
#include "monolib/lib/CLibHbm.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceFont.hpp"

// External function declarations
extern "C" void func_801F369C(void*);
void func_80138078(u32);
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
// func_80137510 declared in code_80135FDC.hpp
extern "C" void func_801D216C(void*, u8);
extern const float lbl_eu_80668B68;
extern const float lbl_eu_80668B6C;
extern const float lbl_eu_80668B70;
extern const float lbl_eu_80668B74;

// Forward declarations for functions called before their definitions
// extern "C" to match retail unmangled reloc names
extern "C" int func_8028E964(CSLCur* cur);
extern "C" int func_8028E998(CSLCur* cur, u8 index);
extern "C" void func_80290844(CSaveLoad* p);
extern "C" void func_802908A4(CSaveLoad* p);
extern "C" void func_80290994(CSaveLoad* p);
extern "C" void func_802907E4(CSaveLoad* p);
extern "C" void func_802910D4(CSaveLoad* p);
extern "C" void func_8028EED8(CSLCur* cur);
extern "C" void func_8028EF24(CSLCur* self);
extern "C" void* __dt__Q22cf7CfAwardFv(cf::CfAward*, int);
extern "C" u8 lbl_eu_80538858[];
extern "C" void func_8009D514(void*);
extern cf::CfAward lbl_eu_806649F8;
extern u8 lbl_eu_80576CF8[];
extern void __register_global_object(void*, void*, void*);
extern void func_8009D414(void*);

// CDeviceFile / MemManager / CSysWin / game system function declarations
extern "C" u32 getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" u8 lbl_eu_8050F7CC[];
extern "C" CSaveLoad* lbl_eu_806649F4;
extern u32 lbl_eu_80662AD0;
extern u32 lbl_eu_80662AC8;
extern u32 lbl_eu_80663E28;
extern "C" int CSysWin_isActive(void*);
extern "C" u32 CSysWin_isReady(void*);
extern "C" int CScrollBar_isVisible(void*);
extern "C" void func_80145018();
extern "C" void func_80142C80();
extern "C" void func_8014A2D0();
extern "C" void invalidateQstFlag();
extern "C" void* getInstance__9CTaskGameFv();
extern "C" void func_800426A8();
extern "C" void func_80084F50__Q22cf13CfGameManagerFv();
extern "C" void func_801F34F4(void*);
extern "C" void func_801F3670(void*, void*);
extern "C" void func_801F36BC(void*, int, int);
extern "C" void func_801F3850(void*, u32);
extern "C" u32 func_8009CF8C(u32);
extern "C" void* allocate_head__Q23mtl10MemManagerFUlUli(u32 handle, u32 size, int align);
extern "C" int CSysWin_getUnk34(void*);
extern "C" void func_8022B8E4(void*);
extern "C" int func_80291C60(int v);

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
    p->p0 = allocate_head__Q23mtl10MemManagerFUlUli(getHandleMEM2__Q23mtl10MemManagerFv(), 0xA000, 0x40);
    p->p4 = allocate_head__Q23mtl10MemManagerFUlUli(getHandleMEM2__Q23mtl10MemManagerFv(), 0xA000, 0x40);
    p->p8 = allocate_head__Q23mtl10MemManagerFUlUli(getHandleMEM2__Q23mtl10MemManagerFv(), 0xA000, 0x40);
}
#pragma optimize_for_size off

extern "C" void func_8028E8A4(UnkStruct_3Ptr* p) {
    mtl::MemManager::deallocate(p->p0);
    mtl::MemManager::deallocate(p->p4);
    mtl::MemManager::deallocate(p->p8);
    lbl_eu_806649F0 = 0;
}

extern "C" void func_8023FA64(void*, int, void (*)(int, int, u8));
extern "C" void func_8028E9E0(int, int, u8);

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

u8 CSaveLoad::func_8028F664() { return mField123; }

extern u8 lbl_eu_8053884C[];

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

// Reset a CSLCur to default state with mFieldC/D/E = 1
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

// Struct with a pointer at +8 that has virtual functions
struct UnkPtrHolder {
    char _pad_00[0x08];
    void* mPtr;       // +0x08 - pointer to object with virtual functions
    void* mField0C;     // +0x0C - pointer to object with float field at +0x10
    void* mField10;     // +0x10 - AnimTransform*
    u8 mField14;      // +0x14
    u8 mField15;      // +0x15 - state byte (0=idle, 1=active)
};

// func_8028EC74 is a C-linkage function in retail (unmangled symbol); declare
// it extern "C" so the call reloc inside func_8028EC28 resolves unmangled.
extern "C" void func_8028EC74(UnkPtrHolder* self);
extern "C" void func_8028EC28(UnkPtrHolder* self);

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

// Implement as extern "C" with mangled name to match retail's codegen pattern
extern "C" void func_8028EA74__6CSLCurFv(CSLCur* self) {
    // Create layout from the ArcResourceAccessor in mField4
    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        (nw4r::lyt::Layout**)&self->mField8,
        (nw4r::lyt::ArcResourceAccessor*)self->mField4,
        (const char*)&lbl_eu_8050F7CC);

    // Create first AnimTransform at the mFieldC region (offset 0x0C)
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        (nw4r::lyt::Layout*)self->mField8,
        (nw4r::lyt::AnimTransform**)&self->mFieldC,
        (nw4r::lyt::ArcResourceAccessor*)self->mField4,
        (char*)&lbl_eu_8050F7CC[0x18]);

    // Create second AnimTransform at the mField10 region (offset 0x10)
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        (nw4r::lyt::Layout*)self->mField8,
        (nw4r::lyt::AnimTransform**)&self->mField10,
        (nw4r::lyt::ArcResourceAccessor*)self->mField4,
        (char*)&lbl_eu_8050F7CC[0x35]);

    // Virtual call at vtable offset 0x24 (slot 9) on the layout
    ((void (*)(void*))(((void**)self->mField8)[0x24 / 4]))(self->mField8);

    // Call func_8028EC74 which continues initialization
    func_8028EC74((UnkPtrHolder*)self);
}

extern "C" void func_8028EAF8(UnkPtrHolder* self) {
    if (self->mPtr == nullptr) return;

    // Use int cast to force signed comparison (cmpwi not cmplwi)
    int state = self->mField15;
    if (state == 0) goto case0;
    if (state == 1) goto case1;
    goto end;

case0:
    func_80137444((nw4r::lyt::AnimTransform*)self->mField0C, lbl_eu_80668B68);
    goto end;

case1:
    func_8028EC28(self);

end:
    // Virtual call at vtable offset 0x38 (slot 14) on mPtr
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

// func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii declared in code_80135FDC.hpp

// Render layout with null + active guards; tail-calls func_80137038
extern "C" void func_8028EB70(CSLCur* self, nw4r::lyt::DrawInfo* drawInfo) {
    nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)self->mField8;
    if (layout == nullptr) return;
    if (self->mField14 == 0) return;
    func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(layout, drawInfo, 0, 1);
}

extern "C" void func_8028EB9C(UnkPtrHolder* self) {
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

void func_8028EC04(void* arg1, const void* arg2) {
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

extern "C" void __dl__FPv(void*);
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __dt__10CScrollBarFv(void*, int);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);

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
extern "C" void func_8028EF74(UnkTwoPtr*);

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
// This is a C-linkage function (retail symbol is unmangled)
extern "C" void func_8028ED70(UnkED70_Struct* s) {
    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        &s->mLayout, s->mAccessor, (const char*)&lbl_eu_8050F7CC[0x50]);
    
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        s->mLayout, &s->mAnimTrans, s->mAccessor, (char*)&lbl_eu_8050F7CC[0x69]);
    
    nw4r::lyt::Pane* rootPane = s->mLayout->GetRootPane();
    void* fontObj = CDeviceFont::func_80452C10(1, s->mLayout);
    void** vtable = *(void***)fontObj;
    u32 (*slot9)(void*) = (u32 (*)(void*))vtable[9];
    u32 result = slot9(fontObj);
    func_8013676C(rootPane, result);
    
    func_8028EF74((UnkTwoPtr*)s);
}

// Dispatch to cursor animation update based on mFieldC state,
// then call virtual slot 14 on the mField4 object
void func_8028EDF8(CSLCur* self) {
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

extern "C" void func_8028EE68(UnkSlot4Ptr* self) {
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

void CSLCur::func_8028EEC0() {
    mFieldC = 1;
    mFieldD = 1;
    mFieldE = 0;
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

extern "C" void func_8028EF74(UnkTwoPtr* self) {
    ((UnkVtblObj*)self->mObjA)->vf11(self->mObjB, 1);
    *(float*)((char*)self->mObjB + 0x10) = lbl_eu_80668B6C;
    ((UnkVtblObj*)self->mObjA)->vf14(0);
}

void __ct__CSaveLoad(){}

// Implement destructor as extern "C" with the mangled name to match retail's register save pattern
// (the header's virtual ~CSaveLoad() declaration still provides the vtable entry)
extern "C" CSaveLoad* __dt__9CSaveLoadFv(CSaveLoad* self, int flags) {
    if (self != nullptr) {
        __dt__7CSysWinFv((u8*)self + 0xD4, -1);
        __dt__7CSysWinFv((u8*)self + 0x98, -1);
        __dt__10CScrollBarFv(self->mScrollbar, -1);
        __dt__6CCur18Fv((u8*)self + 0x40, -1);
        __dt__17UnkClass_8045F564Fv((u8*)self + 0x04, -1);
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void CSaveLoad::func_8028F23C() {
    u32 handle = getHandleMEM2__Q23mtl10MemManagerFv();
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

void func_8028F2CC(){}

void func_8028F3D4(){}

extern "C" void func_eu_804521BC(int);
extern "C" void func_801F35DC(void*);
extern "C" void func_8022B7F4(void*);

// Cleanup/reset function for CSaveLoad
void func_8028F4AC(CSaveLoad* self) {
    CWorkSystem::setSaveLoadInvalidReset(false);
    func_eu_804521BC(1);
    CLibHbm::func_8045D470(false);
    func_801390E0(&self->mFileHandle);
    self->mField120 = 0;
    func_8028E8A4((UnkStruct_3Ptr*)&self->mCur);
    
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
    func_801F35DC(self->mScrollbar);
    func_8022B7F4(&self->mSysWin98);
    {
        void* obj = *(void**)((char*)self + 0x40);
        if (obj != nullptr) {
            ((UnkObj*)obj)->vf2(1);
        }
    }
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

void func_8028F7D0(){}

void func_8028F904(){}

// Handle cursor movement: if global count >= 3, move cursor back by 3 positions
// with underflow handling. Otherwise reset cursor to 0.
void func_8028FA54(CSaveLoad* p) {
    if (p->mField121 != 3) return;
    if (CSysWin_getUnk34((void*)&p->mSysWin98) != 0) return;
    if (CSysWin_getUnk34((void*)&p->mSysWinD4) != 0) return;
    if (p->mField11D != 0) return;

    if ((s32)lbl_eu_80662AC8 >= 3) {
        int sub = (s16)p->mField126 - 3;
        s16 ssub = (s16)sub;
        p->mField126 = (u16)ssub;
        if (ssub < 0) {
            int adj = (int)ssub + 2;
            ssub = 0;
            p->mField124 = (u8)adj;
            u8 bval = (u8)adj;
            if ((s8)bval < 0) {
                p->mField124 = 0;
                p->mField126 = 0;
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

void func_8028FB20(){}

void func_8028FC18(){}

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

void func_8028FECC(){}

// Check save/load state and return a status code:
// 0 = not ready (state mismatch, window active, or sub-window active)
// 0x69 = ready (no cursor) — mField129 == 0, func_8028E998 returns 0
// 0x6A = ready (has cursor) — mField129 == 0, func_8028E998 returns > 0
// 0x6B = ready (has cursor, mField129 != 0) — func_8028E998 returns > 0
int func_8028FFD4(CSaveLoad* p) {
    if (p->mField121 != 3) return 0;
    if (CSysWin_getUnk34((void*)&p->mSysWin98) != 0) return 0;
    if (CSysWin_getUnk34((void*)&p->mSysWinD4) != 0) return 0;
    if (p->mField11D != 0) return 0;

    if (p->mField129 != 0) {
        // Cursor active: return 0x6B if valid (non-zero), 0 otherwise.
        // Call through a cast pointer to keep MWCC from inlining func_8028E998.
        int rv = ((int (*)(CSLCur*, u8))func_8028E998)(&p->mCur, p->mField124);
        return rv ? 0x6B : 0;
    } else {
        // Cursor inactive: return 0x6A if valid (non-zero), 0x69 otherwise
        int rv = ((int (*)(CSLCur*, u8))func_8028E998)(&p->mCur, p->mField124);
        return rv ? 0x6A : 0x69;
    }
}

extern "C" void func_80290094(CSaveLoad* p) {
    if (func_80137444(p->mAnimTransA, lbl_eu_80668B68) != 0) {
        p->mField121 = 2;
        // Use function pointer to prevent inlining
        void (*fn)(CSaveLoad*) = func_80290844;
        fn(p);
    }
}

extern "C" void func_802900E0(CSaveLoad* p) {
    if (func_80137444(p->mAnimTransB, lbl_eu_80668B68) != 0) {
        p->mField121 = 3;
        p->mField3C = 1;
        func_802908A4(p);
        p->mField123 = 1;
    }
}

extern "C" void func_80290140(CSaveLoad* self) {
    if (func_80137510(self->mAnimTransB, lbl_eu_80668B68) != 0) {
        self->mField121 = 5;
        {
            void (*fn)(CSaveLoad*) = func_802907E4;
            fn(self);
        }
    }
}

extern "C" void func_8029018C(CSaveLoad* self) {
    if (func_80137510(self->mAnimTransA, lbl_eu_80668B68) != 0) {
        self->mField121 = 0;
        self->mField123 = 1;
    }
}

extern "C" void func_802901D8(CSaveLoad* self) {
    if (CSysWin_isActive((void*)&self->mSysWin98) != 0) {
        self->mField121 = 3;
        func_801D216C((char*)self + 0x40, 1);
        {
            void (*fn)(CSaveLoad*) = func_802908A4;
            fn(self);
        }
    }
}

void func_8029022C(){}

void func_8029040C(CSaveLoad* p) {
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

void func_8029049C(CSaveLoad* p) {
    if (p->mField11E != 0) {
        p->mField121 = 3;
    }
}

void func_802904B4(){}

extern "C" void func_8029078C(CSaveLoad* p) {
    // Function pointer to prevent MWCC from inlining func_8028E964
    int (*checkFn)(CSLCur*) = func_8028E964;
    if (checkFn(&p->mCur) != 0) {
        p->mField121 = 0xb;
        p->mField11C = 3;
        p->mField11E = 0;
        func_80290994(p);
    }
}

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
extern "C" int func_8028E998(CSLCur* cur, u8 index) {
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

void func_802908A4(){}

void func_80290994(){}

void func_802910D4(){}

extern "C" void code80135FDC_thunk_BFE8C(u8*);

// Load global CSaveLoad* from lbl_eu_806649F4. Takes 4 parameters (r3-r6)
// but only uses r5 (flag: non-zero = handle mField12C/mField129 branches + play sound)
// and r6 (byte value stored into mField12F when r5 == 0).
// r3 and r4 are unused params — they exist so r5/r6 are in the correct register slots.
// No local variable for the global pointer — access lbl_eu_806649F4 directly
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

void CSaveLoad::OnFileEvent() {}

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
extern "C" void func_8027EEF4(u32);

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

extern "C" u32 func_8027EE88(u32, u32);
extern "C" void func_8027EEF4(u32);

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
extern "C" u32 func_80291D98();
extern "C" u32 func_80292000();
extern "C" u32 func_80292418();
extern "C" u32 func_802929C8();
extern "C" u32 func_80292EC0();
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
extern "C" int func_800824FC__Q22cf13CfGameManagerFv(int, int);

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
int func_80291BF8() {
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

extern "C" int func_80291C60(int v) {
    if (v < 0) return 0;
    switch (v) {
    case 0xf: case 0x10: case 0x12: case 0x13:
    case 0x6d: case 0x6e: case 0x70:
    case 0x8b: case 0x8d:
    case 0xe3: case 0xe5: case 0xe7:
    case 0x12c:
        return 0;
    default:
        return 1;
    }
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



// sinit: static initializer for the global CfAward instance at lbl_eu_806649F8
void sinit_802930E0() {
    // Set the manually-managed vtable pointers, call the constructor, and
    // register the atexit destructor for the global CfAward instance.
    void* self = &lbl_eu_806649F8;
    void* base = (void*)&lbl_eu_80538858;
    void* dtor = (void*)__dt__Q22cf7CfAwardFv;
    ((cf::CfAward*)self)->mSecondBase = base;
    ((cf::CfAward*)self)->mVtbl = (char*)base + 8;
    func_8009D414(self);
    __register_global_object(self, dtor, (void*)&lbl_eu_80576CF8);
}

extern "C" void func_8028EC74(UnkPtrHolder* self) {
    // Virtual dispatch through inline vtable access to match retail's register allocation
    // (no local variable for the vtable pointer, uses r3 for this in each call)
    ((void (*)(void*, void*))((void**)*(void**)self->mPtr)[0x20 / 4])(self->mPtr, self->mField10);
    ((void (*)(void*, void*))((void**)*(void**)self->mPtr)[0x1C / 4])(self->mPtr, self->mField0C);
    ((void (*)(void*, void*, int))((void**)*(void**)self->mPtr)[0x2C / 4])(self->mPtr, self->mField0C, 1);
    *(float*)((char*)self->mField0C + 0x10) = lbl_eu_80668B6C;
    ((void (*)(void*, int))((void**)*(void**)self->mPtr)[0x38 / 4])(self->mPtr, 0);
}
