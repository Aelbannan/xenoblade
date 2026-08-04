// Auto-scaffolded catalog TU for kyoshin/CSaveLoad
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CSaveLoad.hpp"
#include "monolib/util/MemManager.hpp"

// External function declarations
void func_801F369C(void*);
void func_80138078(u32);
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
extern float lbl_eu_80668B68;
extern float lbl_eu_80668B6C;
extern float lbl_eu_80668B70;
extern float lbl_eu_80668B74;

// Forward declarations for functions called before their definitions
// extern "C" to match retail unmangled reloc names
extern "C" int func_8028E964(CSLCur* cur);
extern "C" void func_80290844(CSaveLoad* p);
extern "C" void func_802908A4(CSaveLoad* p);
extern "C" void func_80290994(CSaveLoad* p);
extern "C" void func_802907E4(CSaveLoad* p);
extern "C" void func_802910D4(CSaveLoad* p);
extern "C" void* __dt__Q22cf7CfAwardFv(cf::CfAward*, int);
extern "C" u8 lbl_eu_80538858[];
extern "C" void func_8009D514(void*);

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

// Struct with three heap pointers at offsets 0, 4, 8
struct UnkStruct_3Ptr {
    void* p0;
    void* p4;
    void* p8;
};

extern u32 lbl_eu_806649F0;

void func_8028E8A4(UnkStruct_3Ptr* p) {
    mtl::MemManager::deallocate(p->p0);
    mtl::MemManager::deallocate(p->p4);
    mtl::MemManager::deallocate(p->p8);
    lbl_eu_806649F0 = 0;
}

u8 CSaveLoad::func_8028F664() { return mField123; }

void CSLCur::func_8028EA74() {}

void func_8028EAF8(){}

void func_8028EB70(){}

// Struct with a pointer at +8 that has virtual functions
struct UnkPtrHolder {
    char _pad_00[0x08];
    void* mPtr;       // +0x08 - pointer to object with virtual functions
    void* mField0C;     // +0x0C - pointer to object with float field at +0x10
    void* mField10;     // +0x10
    u8 mField14;      // +0x14
};

void func_8028EB9C(UnkPtrHolder* self) {
    void* ptr = self->mPtr;
    self->mField14 = 0;
    self->mField0C = 0;
    self->mField10 = 0;

    if (ptr != nullptr) {
        // Call virtual function at vtable offset 0x8 (index 2)
        // with arg 1 (deleting destructor pattern)
        typedef void (*VFunc)(void*, int);
        VFunc vf = (VFunc)((void**)(*(void**)ptr))[2];
        vf(ptr, 1);
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

void func_8028EC28(){}

void func_8028EC74(UnkPtrHolder* self) {
    // Load vtable pointer once, use self->mPtr as this pointer
    void* vtable = *(void**)self->mPtr;
    ((void (*)(void*, void*))((void**)vtable)[0x20 / 4])(self->mPtr, self->mField10);
    ((void (*)(void*, void*))((void**)vtable)[0x1C / 4])(self->mPtr, self->mField0C);
    ((void (*)(void*, void*, int))((void**)vtable)[0x2C / 4])(self->mPtr, self->mField0C, 1);
    *(float*)((char*)self->mField0C + 0x10) = lbl_eu_80668B6C;
    ((void (*)(void*, int))((void**)vtable)[0x38 / 4])(self->mPtr, 0);
}

void func_8028ED0C(CSLCur* data, int r4) {
    data->mField0 = r4;
    data->mField4 = 0;
    data->mField8 = 0;
    data->mFieldC = 0;
    data->mFieldD = 0;
    data->mFieldE = 1;
}

extern "C" void __dl__FPv(void*);

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

void func_8028ED70(){}

void func_8028EDF8(){}

void func_8028EE68(){}

void CSLCur::func_8028EEC0() {
    mFieldC = 1;
    mFieldD = 1;
    mFieldE = 0;
}

void func_8028EED8(){}

void func_8028EF24(){}

// Struct with two pointer fields at +4 and +8
struct UnkTwoPtr {
    char _pad_00[0x04];
    void* mObjA;  // +0x04 - object with virtual functions at offsets 0x2C and 0x38
    void* mObjB;  // +0x08 - object with a float field at +0x10
};

void func_8028EF74(UnkTwoPtr* self) {
    extern float lbl_eu_80668B6C;

    // Single-expression approach to minimize register pressure
    ((void (*)(void*, void*, int))((void**)(*(void**)self->mObjA))[0x2C / 4])(self->mObjA, self->mObjB, 1);

    *(float*)((char*)self->mObjB + 0x10) = lbl_eu_80668B6C;

    ((void (*)(void*, int))((void**)(*(void**)self->mObjA))[0x38 / 4])(self->mObjA, 0);
}

void __ct__CSaveLoad(){}

CSaveLoad::~CSaveLoad() {}

void CSaveLoad::func_8028F23C() {
    u32 handle = getHandleMEM2__Q23mtl10MemManagerFv();
    mFileHandle = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii(handle, (const char*)&lbl_eu_8050F7CC[0x92], this, 0, 0);
    func_801F34F4(mScrollbar);

    // Call vtable[0x22] (offset 0x88) on the CSysWin-like objects at offsets 0x98 and 0xD4
    // Use explicit vtable pointer variable to match retail r12 dispatch
    {
        void* vt98 = *(void**)&mSysWin98;
        ((void (*)(void*))(((void**)vt98)[0x88 / 4]))((void*)&mSysWin98);
    }
    {
        void* vtD4 = *(void**)&mSysWinD4;
        ((void (*)(void*))(((void**)vtD4)[0x88 / 4]))((void*)&mSysWinD4);
    }

    mField122 = 0;
    lbl_eu_806649F4 = this;
    lbl_eu_80662AD0 = -1;
}

void func_8028F2CC(){}

void func_8028F3D4(){}

void func_8028F4AC(){}

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


void func_8028F66C(){}

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

void func_8028F774(CSaveLoad* p) {
    if (p->mField121 == 3) {
        p->mField121 = 4;
        p->mField123 = 0;
        // Function pointer to prevent inlining
        void (*fn)(CSaveLoad*) = func_80290844;
        fn(p);
        p->mField3C = 0;
        func_801F369C(p->mScrollbar);
        func_80138078(6);
    }
}

void func_8028F7D0(){}

void func_8028F904(){}

void func_8028FA54(){}

void func_8028FB20(){}

void func_8028FC18(){}

void func_8028FE50(){}

u8 CSaveLoad::func_8028FEC4() { return mField12A; }

void func_8028FECC(){}

void func_8028FFD4(){}

void func_80290094(){}

void func_802900E0(CSaveLoad* p) {
    if (func_80137444(p->mAnimTransB, lbl_eu_80668B68) != 0) {
        p->mField121 = 3;
        p->mField3C = 1;
        func_802908A4(p);
        p->mField123 = 1;
    }
}

void func_80290140(){}

void func_8029018C(){}

void func_802901D8(){}

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

void func_8029078C(CSaveLoad* p) {
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

void func_80291204(){}

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

void func_802918AC(){}

void func_8029194C(){}

extern "C" u32 func_8027EE88(u32, u32);
extern "C" void func_8027EEF4(u32);

void func_802919A0() {
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

void func_80291A04(){}

void cf::CfAward::func_80291B18() {}

void func_80291B30(){}

void func_80291BF8(){}

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

void func_80291D98(){}

void func_80291EF0(){}

void func_80292000(){}

void func_80292418(){}

void func_802929C8(){}

void func_80292EC0(){}

// sinit: static initializer for the global CfAward instance at lbl_eu_806649F8
void sinit_802930E0() {
    extern cf::CfAward lbl_eu_806649F8;
    extern void* lbl_eu_80576CF8;
    extern void __register_global_object(void*, void*, void*);
    extern void func_8009D414(void*);

    // Store directly to the global object to generate sda21 relocations
    lbl_eu_806649F8.mSecondBase = (void*)lbl_eu_80538858;
    lbl_eu_806649F8.mVtbl = (void*)(lbl_eu_80538858 + 8);

    // Call the constructor
    func_8009D414(&lbl_eu_806649F8);

    // Register the destructor for atexit cleanup
    __register_global_object(&lbl_eu_806649F8, (void*)__dt__Q22cf7CfAwardFv, (void*)&lbl_eu_80576CF8);
}
