// Auto-scaffolded catalog TU for kyoshin/CCollepedia
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CCollepedia.hpp"
#include "nw4r/lyt.h"
#include "kyoshin/code_80135FDC.hpp"

// Extern C-linkage helpers from other TUs
extern "C" {
    void code80135FDC_setVec3(float*, float, float, float);
    void __dl__FPv(void*);
    u32 CSysWin_isReady(u8*);
    u32 CSysWin_getUnk34(u8*);
    int CSysWin_isActive(u8*);
    u32 func_801B481C();
    u32 func_80158068(u16);
    extern "C" char lbl_eu_8050C6E8[];
    extern void* lbl_eu_806640A8;
    extern void* lbl_eu_80664104;
    extern const float lbl_eu_80668800;
    extern const float lbl_eu_806687F0;
    extern const float lbl_eu_806687F4;
    extern const float lbl_eu_806687F8;
    extern const float lbl_eu_806687FC;
    void func_80138078__FUl(u32);
    extern void* lbl_eu_80537474[];
void func_801D20B0(CBaseCur*, nw4r::lyt::DrawInfo*);
void func_8022B7C8(void*, nw4r::lyt::DrawInfo*);
    u32 func_80136254(char*, char*, u16);
    void func_8013E2E0(u32, u32, u32, u32, u32, u32, u32, u32, u32);
    void func_80137924(void*, void*, void*, void*);
    extern char* lbl_eu_806647DC;
    extern void* lbl_eu_806647D8;
    extern "C" u32 func_8009EC6C(u16);
    extern "C" u16 lbl_eu_8050C6A0[];
    void func_801D216C(void*, u8);
    extern u32 lbl_eu_806640EC;
    void __dt__7CSysWinFv(void*, int);
    void __dt__6CCur18Fv(void*, int);
    void __dt__6CCur07Fv(void*, int);
    void __dt__17UnkClass_8045F564Fv(void*, int);
    extern "C" void func_8022B9B4(void*, void*, int);
    extern "C" void func_8022BFC8(void*, int);
    extern "C" void func_8022B8B8(void*);
    extern "C" void func_8022B8E4(void*);
    extern "C" void func_8022B7F4(void*);
    extern "C" void func_8003AA8C__5CBdatFUl(u32);
    extern "C" void func_801390E0__FPP11CFileHandle(void*);
    extern "C" void func_8045F778__17UnkClass_8045F564Fv(void*);
    extern "C" void func_8009EC18(u16, u32);
    extern "C" u32 func_801587E8(u16);
    extern "C" void* CItem_initItemImplInstances();
    extern "C" void func_80158118(void*, u16, u32);
    extern "C" void func_8013B428__FUl(u32);
    extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
    extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
    extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
    extern "C" u32 func_800A9D90();
    extern "C" float func_801895EC();
    extern "C" void func_80043738(u32, const char*, void*, u32, u32, u32);
    extern "C" void func_80124270(nw4r::lyt::Pane*, u32);
    extern "C" void copyVEC3(void*, void*);
}

// These functions are declared in code_80135FDC.hpp

int sprintf(char*, const char*, ...);

// Forward declarations for stubs defined at end of file
extern "C" void func_8025406C(u8*);
extern "C" u8 func_802540DC(u8*);
extern "C" u32 func_80254144(u8*);
extern "C" __declspec(noinline) void func_80255F98(CCollepedia*);
extern "C" __declspec(noinline) void func_8025641C();
extern "C" void func_8025629C(CCollepedia*);
extern "C" void func_80256314(CCollepedia*);
extern "C" void func_8025516C(CCollepedia*);
extern "C" void func_80255210(CCollepedia*);
// Forward declarations for functions defined before target callers
extern "C" __declspec(noinline) void func_80253A14(CCollepedia*);
extern "C" __declspec(noinline) void func_80253A60(CCollepedia*);
extern "C" void func_802533F4(CCollepedia*);
extern "C" u8 func_8025440C(u8*, u32, u32);
extern "C" void func_802545C0(u8*);


// C-linkage declarations for functions from code_80135FDC.cpp to avoid reloc name drift
// (provided by code_80135FDC.hpp in the current tree)

// CCLPCur constructor - sets vtable, stores accessor, zeros everything
extern "C" void __ct__CCLPCur(CCLPCur* self, nw4r::lyt::ArcResourceAccessor* pAccessor) {
    self->mVtable = lbl_eu_80537474;
    self->mArcResAcc = pAccessor;
    self->mpLayout = nullptr;
    self->mpAnimTrans0 = nullptr;
    self->mpAnimTrans1 = nullptr;
    self->mActive = 0;
    self->mVisible = 0;
}

extern "C" void* __dt__802531C4(void* self, int dealloc_flag) {
    void* result = self;
    if (result != nullptr && dealloc_flag > 0) {
        __dl__FPv(result);
    }
    return result;
}

// Target 2: us-80255440
// Initialize CCollepedia layout: load layout, bind animations, position panes
void func_80253204(CCollepedia* this_) {
    func_80136E84(&this_->field_8,
        (nw4r::lyt::ArcResourceAccessor*)this_->field_4,
        &lbl_eu_8050C6E8[0x00]);
    func_80136F08(this_->field_8, &this_->field_c_ptr,
        (nw4r::lyt::ArcResourceAccessor*)this_->field_4,
        &lbl_eu_8050C6E8[0x18]);

    // Set up position for first pane
    float vec1[3];
    code80135FDC_setVec3(vec1, lbl_eu_806687F0, lbl_eu_806687F4, lbl_eu_806687F8);

    // Find first pane and copy position
    nw4r::lyt::Pane* pane1 = this_->field_8->GetRootPane()->FindPaneByName(
        &lbl_eu_8050C6E8[0x35], true);
    copyVEC3((float*)((u8*)pane1 + 0x2C), vec1);

    // Set up position for second pane
    float vec2[3];
    code80135FDC_setVec3(vec2, lbl_eu_806687FC, lbl_eu_806687F4, lbl_eu_806687F8);

    // Find second pane and copy position
    nw4r::lyt::Pane* pane2 = this_->field_8->GetRootPane()->FindPaneByName(
        &lbl_eu_8050C6E8[0x41], true);
    copyVEC3((float*)((u8*)pane2 + 0x2C), vec2);

    // Call UnbindAllAnimation, then bind specific ones
    this_->field_8->UnbindAllAnimation();

    func_802533F4(this_);
}

extern "C" void func_802532FC(CBaseCur* this_) {
    if (this_->mpLayout == nullptr) return;
    if (this_->mVisible == 0) {
        func_80137444(this_->mpAnimTrans0, lbl_eu_80668800);
    }
    this_->mpLayout->Animate(0);
}

// Target 5: us-80257984
// Check if animation at field_3C is done, then disable/enable animations and set state
extern "C" void func_80255748(CCollepedia* this_) {
    if (func_80137444(this_->field_3C, lbl_eu_80668800) != 0) {
        this_->field_38->SetAnimationEnable(this_->field_44, false);
        this_->field_38->SetAnimationEnable(this_->field_3C, false);
        this_->field_38->SetAnimationEnable(this_->field_40, true);
        this_->field_49 = 2;
    }
}

// Target 5: Render layout if active and non-null
extern "C" __declspec(noinline) void func_80253360(CBaseCur* this_, nw4r::lyt::DrawInfo* drawInfo) {
    if (this_->mpLayout == nullptr) return;
    if (this_->mActive == 0) return;
    func_80137038(this_->mpLayout, drawInfo, 0, 1);
}

extern "C" void func_8025338C(CBaseCur* this_) {
    this_->mActive = 0;
    this_->mpAnimTrans0 = nullptr;
    this_->mpAnimTrans1 = nullptr;
    if (this_->mpLayout != nullptr) {
        delete this_->mpLayout;
        this_->mpLayout = nullptr;
    }
}

// Target 4: us-80255630
// Unbind animation, bind new animation, enable animation, set frame to 0, animate
extern "C" __declspec(noinline) void func_802533F4(CCollepedia* this_) {
    this_->field_8->UnbindAnimation(this_->field_10_ptr);
    this_->field_8->BindAnimation(this_->field_c_ptr);
    this_->field_8->SetAnimationEnable(this_->field_c_ptr, true);
    this_->field_c_ptr->SetFrame(lbl_eu_806687F8);
    this_->field_8->Animate(0);
}

void CCollepedia_reset(CCollepedia* obj, int val) {
    obj->field_0 = val;
    obj->field_4 = 0;
    obj->field_8 = 0;
    obj->field_c_ptr = 0;
    reinterpret_cast<u8*>(&obj->field_c_ptr)[1] = 1;
}

extern "C" void* __dt__802534B0(void* self, int dealloc_flag) {
    void* result = self;
    if (result != nullptr && dealloc_flag > 0) {
        __dl__FPv(result);
    }
    return result;
}

void func_802534F0(){}

// Target 4: Delete the layout at field_4 if present, then clear the pointer
extern "C" void func_80253794(CCollepedia* this_) {
    if (this_->field_4 != nullptr) {
        delete this_->field_4;
        this_->field_4 = nullptr;
    }
}

// Target 2: If layout is loaded, handle state machine and animate
extern "C" void func_802537EC(CCollepedia* this_) {
    if (this_->field_4 == nullptr) {
        return;
    }
    u8 state = reinterpret_cast<u8*>(&this_->field_c_ptr)[0];
    switch ((int)state) {
    case 1:
        func_80253A14(this_);
        break;
    case 3:
        func_80253A60(this_);
        break;
    }
    this_->field_4->Animate(0);
}

// Target 1: Initialize state and call scheduler if field_c is 0
extern "C" void func_8025385C(CCollepedia* this_) {
    u8* stateBytes = reinterpret_cast<u8*>(&this_->field_c_ptr);
    if (stateBytes[0] != 0) return;
    stateBytes[0] = 1;
    stateBytes[1] = 0;
    stateBytes[2] = 1;
    func_80138078__FUl(0xD);
}

// Target 3: Return byte at +0xD from a sub-array entry (indexed by arg)
extern "C" u32 func_8025415C(u8* obj, u32 index) {
    if (index >= 6) return 0;
    s8 idx = (s8)obj[1];
    return obj[idx * 0x140 + index * 0x34 + 0xD];
}

// Target 4: Return byte at +0xC from a sub-array entry (indexed by arg)
extern "C" u8 func_8025418C(u8* obj, u32 index) {
    if (index >= 6) return 0;
    s8 idx = (s8)obj[1];
    return obj[idx * 0x140 + index * 0x34 + 0xC];
}

// Target 3: If field_c is 2, set it to 3, clear field_d, and schedule event 0xE
extern "C" void func_80253888(CCollepedia* this_) {
    u8* stateBytes = reinterpret_cast<u8*>(&this_->field_c_ptr);
    if (stateBytes[0] != 2) return;
    stateBytes[0] = 3;
    stateBytes[1] = 0;
    func_80138078__FUl(0xE);
}

// Target 1: Set two named panes (0x82, 0x92) on the layout at field_4
extern "C" void func_802538B0(CCollepedia* this_, unsigned char arg) {
    func_80136910(this_->field_4, &lbl_eu_8050C6E8[0x82], (unsigned char)1);
    func_80136910(this_->field_4, &lbl_eu_8050C6E8[0x92], arg);
}

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
extern "C" void func_80253904(CCollepedia* this_, char* arg2, char* arg3) {
    if (arg2 != NULL) {
        func_80137E7C(this_->field_4, &lbl_eu_8050C6E8[0x138], arg2);
    }
    if (arg3 != NULL) {
        func_80137E7C(this_->field_4, &lbl_eu_8050C6E8[0x144], arg3);
    }
}
#pragma pop

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
// Target 2: us-80255bac
// Format a pane name, find two panes by name, and call func_80137924 to copy text
// Retail saves r28-r31 with stmw and uses a 0x40-byte stack frame
// Retail reloads rootPane from *(layout+0x10) each time (no caching)
extern "C" void func_80253970(CCollepedia* this_, LayoutContainer* container, int arg3) {
    char buf[0x28];
    sprintf(buf, &lbl_eu_8050C6E8[0x14f], arg3 + 1);
    nw4r::lyt::Pane* pane1 = ((nw4r::lyt::Pane*)*(void**)((u8*)container->mpLayout + 0x10))->FindPaneByName(buf, true);
    nw4r::lyt::Pane* pane2 = ((nw4r::lyt::Pane*)*(void**)((u8*)container->mpLayout + 0x10))->FindPaneByName(&lbl_eu_8050C6E8[0x15c], true);
    func_80137924(this_, pane1, pane2, *(nw4r::lyt::Pane**)((u8*)container->mpLayout + 0x10));
}
#pragma pop

// Target 4: Check if anim transform is finished; if so, set state to 2
// field_8 is a Layout* but used as AnimTransform* in this context
extern "C" __declspec(noinline) void func_80253A14(CCollepedia* self) {
    if (func_80137444(reinterpret_cast<nw4r::lyt::AnimTransform*>(self->field_8), lbl_eu_80668800) != 0) {
        u8* stateBytes = reinterpret_cast<u8*>(&self->field_c_ptr);
        stateBytes[0] = 2;
        stateBytes[1] = 1;
    }
}


// Target 1: If the anim transform at field_8 is not finished, reset the state flags
// field_8 is a Layout* but used as AnimTransform* in this context
extern "C" __declspec(noinline) void func_80253A60(CCollepedia* this_) {
    if (func_80137510(reinterpret_cast<nw4r::lyt::AnimTransform*>(this_->field_8), lbl_eu_80668800) != 0) {
        u8* stateBytes = reinterpret_cast<u8*>(&this_->field_c_ptr);
        stateBytes[0] = 0;
        stateBytes[1] = 1;
        stateBytes[2] = 0;
    }
}

// Target 1: Init the sub-array header (count=0, index=-1) and zero the entry storage
extern "C" s8* func_80253AB0(s8* this_) {
    this_[0] = 0;
    this_[1] = -1;
    memset(this_ + 4, 0, 0x2800);
    return this_;
}

extern "C" void* __dt__80253AFC(void* self, int dealloc_flag) {
    void* result = self;
    if (result != nullptr && dealloc_flag > 0) {
        __dl__FPv(result);
    }
    return result;
}

void func_80253B3C(u8* self_){}

bool func_80253EE8(u8* this_, u32 arg1, u32 arg2) {
    if (arg1 >= 6) return false;
    if (arg2 >= 5) return false;
    s8 idx = (s8)this_[1];
    u8 val = *(this_ + idx * 0x140 + arg1 * 0x34 + arg2 * 0xA + 0x16);
    return val == 2;
}

// Target 4: us-80256178
// Handle item selection in the collepedia grid
void func_80253F3C(u8* self, u32 arg2, u32 arg3) {
    if (arg2 >= 6) return;
    if (arg3 >= 5) return;

    s8 idx = (s8)self[1];
    u32 rowOff = arg2 * 0x34;
    u32 colOff = arg3 * 0xA;
    u8* base = self + idx * 0x140;
    u8* entry = base + rowOff + colOff;

    // Check if item is unlocked
    func_8009EC18(*(u16*)(entry + 0x10), 1);

    // Set entry byte at +0x16 to 3
    entry[0x16] = 3;

    u8 count = (u8)func_8025440C(self, arg2, arg3);
    u8 countMinus1 = count - 1;

    u16 val = *(u16*)(entry + 0xE);
    func_801587E8(val);

    if (countMinus1 > 0) {
        func_80158118(self, val, countMinus1);
    } else {
        void* itemInst = CItem_initItemImplInstances();
        ((void (*)(void*, u16))(*(void***)itemInst)[0x10 / 4])(itemInst, val);
    }

    func_802545C0(self);
}

// Target 2: Increment sub-array counter, wrap to 0 at max
extern "C" __declspec(noinline) void func_80254040(u8* obj) {
    obj[1]++;
    if ((s8)obj[1] >= (s32)obj[0]) {
        obj[1] = 0;
    }
}

void CCollepedia_decrementWrap(unsigned char* obj) {
    obj[1] = obj[1] - 1;
    if ((signed char)obj[1] < 0) {
        obj[1] = obj[0] - 1;
    }
}

extern "C" char* func_80254094(u8* self) {
    s8 idx = (s8)self[1];
    u8* base = self + idx * 0x140;
    u8 val = base[4];
    if (val == 0x1a) {
        return (char*)func_80136190((const void*)&lbl_eu_8050C6E8[0xA2], (const void*)&lbl_eu_8050C6E8[0xAE], 0x14);
    } else {
        return (char*)func_8013639C((const void*)lbl_eu_806640A8, (const void*)&lbl_eu_8050C6E8[0xAE], val);
    }
}

unsigned char CCollepedia_getFieldAtIdx(u8* thisPtr) {
    s8 idx = (s8)thisPtr[1];
    u8* base = thisPtr + idx * 320;
    return base[4];
}

// Target 2: Return a formatted string for the entry at `index` if its byte at +0xC is set
extern "C" char* func_802540F4(u8* self, u32 index) {
    if (index >= 6) return 0;
    s8 idx = (s8)self[1];
    u8 val = self[idx * 0x140 + index * 0x34 + 0xC];
    if (val != 0) {
        return (char*)func_80136190((const void*)&lbl_eu_8050C6E8[0xA2], (const void*)&lbl_eu_8050C6E8[0xAE], val);
    }
    return 0;
}

unsigned char CCollepedia_getFieldOffset(const unsigned char* ptr) {
    int idx = (signed char)ptr[1];
    return ptr[5 + idx * 0x140];
}

// Target 3: Return byte at +0x16 from a sub-sub-array entry (indexed by arg2, arg3)
// Computes arg2*0x34 first to match retail register order
extern "C" u8 func_802541BC(u8* self, u32 arg2, u32 arg3) {
    if (arg2 >= 6) return 0;
    if (arg3 >= 5) return 0;
    s8 idx = (s8)self[1];
    u32 rowOff = arg2 * 0x34;
    u8* ptr = self + idx * 0x140;
    ptr = ptr + rowOff;
    return ptr[arg3 * 0xA + 0x16];
}

u16 CCollepedia_getField12(u8* p1, u32 arg2, u32 arg3) {
    if (arg2 >= 6) return 0;
    if (arg3 >= 5) return 0;
    s8 idx = (s8)p1[1];
    u8* ptr = p1 + idx * 0x140 + arg2 * 0x34 + arg3 * 0xa;
    return reinterpret_cast<CCollepediaEntry*>(ptr)[1].field_08;
}

extern "C" u32 func_8025424C(u8* this_, u32 arg2, u32 arg3) {
    if (arg2 >= 6) return 0;
    if (arg3 >= 5) return 0;
    s8 idx = (s8)this_[1];
    u8* ptr = this_ + idx * 0x140;
    ptr += arg2 * 0x34;
    ptr += arg3 * 0x0A;
    u16 val = *(u16*)(ptr + 0x0E);
    if (val == 0) return 0;
    return func_80136254((char*)lbl_eu_806640EC, &lbl_eu_8050C6E8[0x185], val);
}

// Target 1: Load a float from offset 8 of the current sub-array entry
extern "C" float func_802542B8(u8* this_) {
    s8 idx = (s8)this_[1];
    return *(float*)(this_ + idx * 0x140 + 8);
}

// Target 2: us-8025650c
// Look up an entry in the sub-array, check +0x10 u16 and +0x16 byte.
// If byte != 1, resolve string via func_8013639C.
extern "C" char* func_802542D0(u8* self, u32 arg1, u32 arg2) {
    if (arg1 >= 6) return nullptr;
    if (arg2 >= 5) return nullptr;
    s8 idx = (s8)self[1];
    u8* base = self + idx * 0x140;
    u8* entry = base + arg1 * 0x34 + arg2 * 0xA;
    u16 val = *(u16*)(entry + 0x10);
    if (val == 0) return nullptr;
    u8 b = entry[0x16];
    if (b == 1) return nullptr;
    return (char*)func_8013639C(lbl_eu_80664104, &lbl_eu_8050C6E8[0xAE], val);
}

// Target 4: us-8025658c
// Look up an entry in the sub-array, check if item is unlocked (via func_8009EC6C), format its name
// Returns nullptr if the entry is invalid or locked
extern "C" char* func_80254350(u8* self, u32 arg2, u32 arg3) {
    if (lbl_eu_806647D8 == NULL) return NULL;
    if (arg2 >= 6) return NULL;
    if (arg3 >= 5) return NULL;

    s8 idx = (s8)self[1];
    u8* entry = self + idx * 0x140 + arg2 * 0x34 + arg3 * 0xA;

    u16 val = *(u16*)(entry + 0x14);
    if (val == 0) return NULL;

    u16 check = *(u16*)(entry + 0x10);
    if (func_8009EC6C(check) != 0) {
        return (char*)func_8013639C(lbl_eu_806647D8, &lbl_eu_8050C6E8[0x18f], val);
    }
    return NULL;
}

// Target 3: Read u16 at offset 0xE within the indexed sub-array entry
// If non-zero, resolve via func_80158068 and return as u8
// The sub-array is indexed by: (s8)self[1] * 0x140 + arg2 * 0x34 + arg3 * 0xA + 0xE
extern "C" u8 func_8025440C(u8* self, u32 arg2, u32 arg3) {
    if (arg2 >= 6) return 0;
    if (arg3 >= 5) return 0;
    s8 idx = (s8)self[1];
    self += idx * 0x140;
    self += arg2 * 0x34;
    u16 val = *(u16*)(self + arg3 * 0xA + 0xE);
    if (val != 0) {
        return (u8)func_80158068(val);
    }
    return 0;
}

// Target 2: Load a byte from offset 6 of the current sub-array entry
extern "C" u32 func_80254484(u8* this_) {
    s8 idx = (s8)this_[1];
    u8* base = this_ + idx * 0x140;
    return base[6];
}

void func_8025449C(){}

void func_802545C0(){}

// Target 3: us-80256ae4
// Destructor for CCollepedia.
// Calls sub-object destructors and frees memory if dealloc_flag > 0.
#pragma optimize_for_size on
extern "C" void* __dt__11CCollepediaFv(CCollepedia* self, int flag) {
    if (self) {
        __dt__7CSysWinFv((u8*)self + 0x9C, -1);
        __dt__6CCur18Fv((u8*)self + 0x84, -1);
        __dt__6CCur07Fv((u8*)self + 0x54, -1);
        __dt__17UnkClass_8045F564Fv((u8*)self + 0x14, -1);
        __dt__17UnkClass_8045F564Fv((u8*)self + 0x04, -1);
        if (flag > 0) {
            __dl__FPv((u8*)self);
        }
    }
    return self;
}
#pragma optimize_for_size off

void __ct__CCollepedia(){}

// Target 1: us-80256b68
// Initialize CCollepedia: load 3 files, init CSysWin, init sub-array header
void func_8025492C(CCollepedia* this_) {
    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    this_->field_24 = readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)handle, &lbl_eu_8050C6E8[0x19c], (void*)this_, 0, 0);

    handle = getHandleMEM2__Q23mtl10MemManagerFv();
    this_->field_28 = readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)handle, &lbl_eu_8050C6E8[0x1b3], (void*)this_, 0, 0);

    u32 handle2 = func_800A9D90();
    this_->field_2C = readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(
        handle2, &lbl_eu_8050C6E8[0x1cc], (void*)this_, 0, 0);

    // Virtual call on CSysWin at this+0x9C, vtable slot 34 (offset 0x88)
    // Use a local pointer to match retail register allocation
    CSysWin* syswin = (CSysWin*)(&this_->field_9C);
    ((CSysWinProxy*)syswin)->v32();

    // Init sub-array using temp buffer + memcpy to match retail codegen
    u8 tmp[0x2802];
    tmp[0] = 0;
    tmp[1] = (u8)-1;
    memset(tmp + 4, 0, 0x2800);
    this_->field_E8 = tmp[0];
    this_->_E9[0] = tmp[1];
    memcpy(&this_->_E9[3], tmp + 4, 0x2800);

    func_80253B3C(&this_->field_E8);
}

void func_80254A20(){}

#pragma push
#pragma optimize_for_size on
// Target 1: us-80256da0
// Render the CCollepedia display: draw layout, sub-cursors, and syswin if active
// Retail reads field_48/field_49 and field_38 from r3 (original this) before any reg copy
extern "C" void func_80254B64(CCollepedia* this_, nw4r::lyt::DrawInfo* drawInfo) {
    if (this_->field_48 == 0) return;
    if (this_->field_49 == 0) return;
    
    func_80137038(this_->field_38, drawInfo, 0, 1);
    func_80253360((CBaseCur*)((u8*)this_ + 0x6c), drawInfo);
    
    if (this_->field_28F0 != NULL) {
        func_80137038(this_->field_28F0, drawInfo, 0, 1);
    }
    
    func_801D20B0((CBaseCur*)((u8*)this_ + 0x54), drawInfo);
    func_8022B7C8(&this_->field_9C, drawInfo);
    func_801D20B0((CBaseCur*)((u8*)this_ + 0x84), drawInfo);
}
#pragma pop

// Target 5: us-80256e40
// Cleanup CCollepedia: free files, layouts, sub-objects
void func_80254C04(CCollepedia* this_) {
    func_8003AA8C__5CBdatFUl(2);
    func_801390E0__FPP11CFileHandle((CFileHandle**)&this_->field_24);
    func_801390E0__FPP11CFileHandle((CFileHandle**)&this_->field_28);
    func_801390E0__FPP11CFileHandle((CFileHandle**)&this_->field_2C);

    this_->field_48 = 0;
    func_80253794((CCollepedia*)(&this_->_E9[0x2803]));

    // Free layout at field_38 if present
    if (this_->field_38 != nullptr) {
        delete this_->field_38;
        this_->field_38 = nullptr;
    }

    // Free arc resource accessors
    func_80139124(this_->field_30);
    func_80139124(this_->field_34);

    // Free field_4c (pointer with virtual dtor)
    if (this_->field_4C != 0) {
        void* ptr = (void*)this_->field_4C;
        if (ptr != nullptr) {
            ((void (*)(void*, int))(*(void***)ptr)[0x8 / 4])(ptr, 1);
        }
        this_->field_4C = 0;
    }

    // Free memory regions
    func_8045F778__17UnkClass_8045F564Fv((void*)&this_->field_4);
    func_8045F778__17UnkClass_8045F564Fv((void*)&this_->_14[0]);

    // Free sub-object at field_54 (virtual dtor at vtable+0xC)
    void* obj54 = (void*)&this_->field_54;
    ((void (*)(void*, int))(*(void***)obj54)[0xC / 4])(obj54, 1);

    func_8025338C((CBaseCur*)((u8*)this_ + 0x6C));
    func_8022B7F4(&this_->field_9C);

    // Free sub-object at field_84 (virtual dtor at vtable+0xC)
    void* obj84 = (void*)&this_->field_54[0x30];
    ((void (*)(void*, int))(*(void***)obj84)[0xC / 4])(obj84, 1);
}

extern "C" u8 func_80254D0C(CCollepedia* self) {
    if (CSysWin_isReady(&self->field_9C)) {
        return self->field_50;
    }
    return 0;
}

struct CCollepediaState {
    u8 _00[0x49];
    u8 initialized;
    u8 _4A[7];
    u8 field51;
    u8 _52[0x28F9 - 0x52];
    u8 condition;
};

unsigned char CCollepedia_condGetField51(u8* self) {
    CCollepediaState* state = (CCollepediaState*)self;
    return state->condition ? state->field51 : 0;
}

void CCollepedia_initFields(u8* p) {
    CCollepediaState* state = (CCollepediaState*)p;
    if (state->initialized != 0) return;
    state->initialized = 1;
    state->field51 = 0;
}

// Target 3: us-80256fc8
// Close the collepedia detail view: set state, reset sub-object, disable animations, enable outro, play sound
void func_80254D8C(CCollepedia* this_) {
    if (this_->field_28F9 == 0) return;
    if (!CSysWin_isActive(&this_->field_9C)) return;
    if (CSysWin_getUnk34(&this_->field_9C)) return;
    if (this_->field_49 != 3) return;

    this_->field_49 = 4;
    this_->field_51 = 0;
    func_801D216C(&this_->field_54, 0);

    this_->field_38->SetAnimationEnable(this_->field_44, false);
    this_->field_38->SetAnimationEnable(this_->field_3C, false);
    this_->field_38->SetAnimationEnable(this_->field_40, true);

    func_80138078__FUl(6);
}

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
// Target 5: us-802570a0
// Decrement timer counters (field_DA or field_D9) with wrapping, update display, schedule event
void func_80254E64(CCollepedia* this_) {
    if (CSysWin_getUnk34(&this_->field_9C)) return;

    if (this_->field_28FA != 0) {
        if (this_->field_28F9 == 0) return;

        u8 val = this_->field_DA - 1;
        this_->field_DA = val;
        if ((s8)val < 0) {
            this_->field_DA = 1;
        }
        func_80256314(this_);
    } else {
        if (this_->field_49 != 3) return;

        u8 val = this_->field_D9 - 1;
        this_->field_D9 = val;
        if ((s8)val < 0) {
            this_->field_D9 = (u8)(func_80254144(&this_->field_E8) - 1);
        }
        func_80256314(this_);
        func_8025629C(this_);
    }

    func_80138078__FUl(1);
}
#pragma pop

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
// Target 2: us-80257168
// Increment timer-based navigation: increment field_DA with wrap, or advance field_D9 and update display
void func_80254F2C(CCollepedia* this_) {
    if (CSysWin_getUnk34(&this_->field_9C)) return;

    if (this_->field_28FA != 0) {
        if (this_->field_28F9 == 0) return;

        this_->field_DA = this_->field_DA + 1;
        if ((s8)this_->field_DA >= 2) {
            this_->field_DA = 0;
        }
        func_80256314(this_);
    } else {
        if (this_->field_49 != 3) return;

        this_->field_D9 = this_->field_D9 + 1;
        if ((s8)this_->field_D9 >= (s32)(func_80254144(&this_->field_E8) & 0xFF)) {
            this_->field_D9 = 0;
        }
        func_80256314(this_);
        func_8025629C(this_);
    }

    func_80138078__FUl(1);
}
#pragma pop

// Target 5: Timer-based countdown that calls func_80255210 when field_D8 reaches 0
void func_80255000(CCollepedia* this_) {
    if (this_->field_49 != 3) return;
    if (this_->field_28FA != 0) return;
    if (CSysWin_getUnk34(&this_->field_9C)) return;

    u8 d8 = this_->field_D8;
    if ((s8)d8 == 0) {
        func_80255210(this_);
        this_->field_D8 = 4;
        func_80256314(this_);
        func_8025629C(this_);
    } else {
        u8 val = d8 - 1;
        this_->field_D8 = val;
        if ((s8)val < 0) {
            this_->field_D8 = 4;
        }
        func_80256314(this_);
        func_8025629C(this_);
    }

    func_80138078__FUl(1);
}

// Target 3: us-802572f0
// Increment field_D8 with wrap at 5; when field_D8 == 4 call func_8025516C, then update display
void func_802550B4(CCollepedia* this_) {
    if (this_->field_49 != 3) return;
    if (this_->field_28FA != 0) return;
    if (CSysWin_getUnk34(&this_->field_9C)) return;

    if ((s8)this_->field_D8 == 4) {
        func_8025516C(this_);
        this_->field_D8 = 0;
        func_80256314(this_);
        func_8025629C(this_);
    } else {
        u8 val = this_->field_D8 + 1;
        this_->field_D8 = val;
        if ((s8)val >= 5) {
            this_->field_D8 = 0;
        }
        func_80256314(this_);
        func_8025629C(this_);
    }

    func_80138078__FUl(1);
}

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
// Target 3: us-802573a8
// Decrement field_D9 with wrapping, update display, and schedule next event
extern "C" void func_8025516C(CCollepedia* this_) {
    if (this_->field_49 != 3) return;
    if (this_->field_28FA != 0) return;
    if (CSysWin_getUnk34(&this_->field_9C)) return;

    func_80254040(&this_->field_E8);
    u32 count = func_80254144(&this_->field_E8);

    // Force field_D9 sign-extend before comparing with masked count
    int fd9 = (s8)this_->field_D9;
    int cmask = (int)(count & 0xFF);
    if (fd9 >= cmask) {
        this_->field_D9 = (u8)(func_80254144(&this_->field_E8) - 1);
        func_80256314(this_);
    }

    func_80255F98(this_);
    func_8025629C(this_);
    func_80138078__FUl(10);
}
#pragma pop

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
// Target 4: us-8025744c
// Decrement field_D9 with wrapping (calls func_8025406C instead of func_80254040)
extern "C" void func_80255210(CCollepedia* this_) {
    if (this_->field_49 != 3) return;
    if (this_->field_28FA != 0) return;
    if (CSysWin_getUnk34(&this_->field_9C)) return;

    func_8025406C(&this_->field_E8);
    u32 count = func_80254144(&this_->field_E8);

    if ((s8)this_->field_D9 >= (s32)(count & 0xFF)) {
        this_->field_D9 = (u8)(func_80254144(&this_->field_E8) - 1);
        func_80256314(this_);
    }

    func_80255F98(this_);
    func_8025629C(this_);
    func_80138078__FUl(10);
}
#pragma pop

// Target 3: us-802574f0
// Handle CCollepedia navigation: check state and dispatch actions
void func_802552B4(CCollepedia* this_) {
    // Early return for states that skip the CSysWin check
    if (this_->field_49 == 9) return;
    if (this_->field_49 == 0xB) return;
    if (this_->field_49 == 0xC) return;
    if (this_->field_49 == 0xD) return;
    if (this_->field_49 == 0xF) return;
    if (this_->field_49 == 0x10) return;

    if (CSysWin_getUnk34(&this_->field_9C) != 0) {
        if (CSysWin_isActive(&this_->field_9C) != 0) {
            if (this_->field_49 == 0xA) {
                this_->field_49 = 0xB;
            } else if (this_->field_49 == 0xE) {
                this_->field_49 = 0xF;
            }
            func_8022B8E4(&this_->field_9C);
        }
        return;
    }

    // CSysWin is not active, check sub-array conditions
    if (this_->field_28FA == 0) return;
    if (this_->field_28F9 == 0) return;

    func_80253888((CCollepedia*)(&this_->_E9[0x2803]));
    func_801D216C(&this_->field_54[0x30], 0);
    this_->field_49 = 8;
    this_->field_51 = 0;
    func_80138078__FUl(6);
}

void func_802553AC(){}

void func_80255688(void* self) {
    CCollepediaFull* full = static_cast<CCollepediaFull*>(self);
    func_80253EE8((u8*)self + 0xE8, full->field_D9, full->field_D8);
}

extern "C" u8 func_80255698(CCollepedia* self) {
    if (CSysWin_getUnk34(&self->field_9C)) {
        return 1;
    }
    return self->field_28FA;
}

// Target 1: Returns 1 if any block condition is active, otherwise calls func_801B481C
extern "C" u32 func_802556DC(CCollepedia* this_) {
    if (this_->field_49 != 3) {
        return 1;
    }
    if (CSysWin_getUnk34(&this_->field_9C)) {
        return 1;
    }
    if (this_->field_28FA != 0) {
        return 1;
    }
    return func_801B481C();
}

// Target 2: us-80257a1c
// Check if animation at field_40 is done; if so, set state, init sub-object, switch animations
void func_802557E0(CCollepedia* this_) {
    if (func_80137444(this_->field_40, lbl_eu_80668800) != 0) {
        this_->field_49 = 3;
        this_->field_51 = 1;
        func_801D216C(&this_->field_54, 1);

        this_->field_38->SetAnimationEnable(this_->field_3C, false);
        this_->field_38->SetAnimationEnable(this_->field_40, false);
        this_->field_38->SetAnimationEnable(this_->field_44, true);

        func_80256314(this_);
    }
}

// Target 1: us-80257ad0
// Check if animation at field_40 is still playing; if so, disable field_40 and field_44, enable field_3C, set state to 5
void func_80255894(CCollepedia* this_) {
    if (func_80137510(this_->field_40, lbl_eu_80668800) != 0) {
        this_->field_38->SetAnimationEnable(this_->field_40, false);
        this_->field_38->SetAnimationEnable(this_->field_44, false);
        this_->field_38->SetAnimationEnable(this_->field_3C, true);
        this_->field_49 = 5;
    }
}

// Target 5: Check if animation at field_3C is done, then set state and init sub-object
extern "C" void func_8025592C(CCollepedia* this_) {
    if (func_80137510(this_->field_3C, lbl_eu_80668800) != 0) {
        this_->field_51 = 1;
        this_->field_49 = 0;
        func_801D216C(this_->field_54, 0);
    }
}

extern "C" void func_80255984(CCollepedia* this_) {
    if (this_->field_28F9 == 0) return;
    this_->field_49 = 7;
    this_->field_51 = 1;
    func_801D216C(&this_->field_54[0x30], 1);
    func_80256314(this_);
}

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
// Target 4: us-80257c18
// Open the collepedia detail view: set state, enable sub-object, update display, load file
void func_802559DC(CCollepedia* this_) {
    if (this_->field_28F9 == 0) return;

    this_->field_49 = 3;
    this_->field_51 = 1;
    func_801D216C(&this_->field_54, 1);

    func_80256314(this_);

    if (func_8025415C(&this_->field_E8, this_->field_D9) == 0) return;

    const char* strTab = lbl_eu_8050C6E8;
    this_->field_49 = 9;

    char* str = (char*)func_80136190(&strTab[0xA2], &strTab[0xAE], 0x12);
    func_8022B9B4(&this_->field_9C, str, 0);
    func_8022BFC8(&this_->field_9C, 1);
    func_8022B8B8(&this_->field_9C);

    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    func_801895EC();
    func_80043738(0, &strTab[0x1E9], handle, 2, 1, 0);
}
#pragma pop

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
// Target 5: us-80257cf0
// If CSysWin is active, set state, look up table entry, format string, call func_8013E2E0
extern "C" void func_80255AB4(CCollepedia* this_) {
    if (!CSysWin_isActive(&this_->field_9C)) return;

    this_->field_49 = 12;

    u32 idx = func_802540DC(&this_->field_E8);
    // Table is u16 array at lbl_eu_8050C6A0; index u8 then *2 for lhzx byte offset
    u16 tableVal = lbl_eu_8050C6A0[idx & 0xFF];

    // Compute (tableVal + 1) masked to u16, then add signed field_D9
    // Retail order: tableVal+1 -> mask -> + field_D9 (no second mask)
    u32 result = func_80136254(lbl_eu_806647DC, &lbl_eu_8050C6E8[0x1f9],
        (u16)(tableVal + 1 + (s8)this_->field_D9));

    u32 zero = 0;
    func_8013E2E0((u16)result, 0, 0, 0, 0, 0, 0, 0, zero);
}
#pragma pop

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
// Target 1: us-80257d9c
// Initialize collepedia display: check condition, set up CSysWin with text, play sounds, load file
void func_80255B60(CCollepedia* this_) {
    if (func_801B481C()) return;

    this_->field_49 = 3;

    if (func_80254484(&this_->field_E8) == 0) return;

    const char* strTab = lbl_eu_8050C6E8;
    this_->field_49 = 0xD;

    char* str = (char*)func_80136190(&strTab[0xA2], &strTab[0xAE], 0x13);
    func_8022B9B4(&this_->field_9C, str, 0);
    func_8022BFC8(&this_->field_9C, 1);
    func_8022B8B8(&this_->field_9C);

    func_8013B428__FUl(0x85);
    func_8013B428__FUl(0x86);

    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    func_801895EC();
    func_80043738(0, &strTab[0x200], handle, 2, 1, 0);
}
#pragma pop

// Target 2: us-80257e64
// If CSysWin is active, do a table lookup and format a sound/effect call
void func_80255C28(CCollepedia* this_) {
    if (!CSysWin_isActive(&this_->field_9C)) return;
    
    this_->field_49 = 0x10;
    
    u32 idx = func_802540DC(&this_->field_E8);
    u32 idxTrunc = (idx & 0xFF);
    u16 tableVal = lbl_eu_8050C6A0[idxTrunc];
    
    char* str = &lbl_eu_8050C6E8[0x1f9];
    u32 result = func_80136254(lbl_eu_806647DC, str, tableVal);
    func_8013E2E0((u16)result, 0, 0, 0, 0, 0, 0, 0, 0);
}

// Target 1: us-80257efc
// Check layout and pointer chain, then update display if valid
// Reads a pointer from field_34, checks its first word, then checks offset +4 of that word
void func_80255CC0(CCollepedia* this_) {
    if (this_->field_38 == nullptr) return;
    if (this_->field_34 == nullptr) return;
    u32 ptr = *(u32*)this_->field_34;
    if (ptr == 0) return;
    if (*(u32*)(ptr + 4) != 0) {
        func_8025641C();
        func_80255F98(this_);
        func_8025629C(this_);
        this_->field_50 = 1;
        this_->field_48 = 1;
    }
}

void func_80255D3C(){}

void func_80255E90(){}

#pragma push
#pragma optimize_for_size on
// Target 5: us-80258658
// Iterate sub-array entries, set pane visibility for each, copy position for current entry, update display
void func_8025641C(CCollepedia* this_) {
    u8 count = this_->field_E8;
    for (u8 i = 1; i <= 0x16; i++) {
        char buf[0x28];
        sprintf(buf, &lbl_eu_8050C6E8[0x2F7], i);

        nw4r::lyt::Pane* pane = this_->field_38->GetRootPane()->FindPaneByName(buf, true);
        func_80124270(pane, i <= count);

        if (i == count) {
            nw4r::lyt::Pane* curPane = this_->field_38->GetRootPane()->FindPaneByName(buf, true);
            copyVEC3(this_->field_DC, (float*)((u8*)curPane + 0x2C));
        }
    }

    func_80136910(this_->field_38, &lbl_eu_8050C6E8[0x306], count);
    func_80136910(this_->field_38, &lbl_eu_8050C6E8[0x284], 1);
}
#pragma pop

// Stubs for functions called by targets
extern "C" void func_8025406C(u8*){}
extern "C" u8 __attribute__((noinline)) func_802540DC(u8* self) {
    s8 idx = (s8)self[1];
    return *(u8*)(self + idx * 0x140 + 4);
}
extern "C" u32 func_80254144(u8*){ return 0; }
extern "C" __declspec(noinline) void func_80255F98(CCollepedia* this_){}
// Target 4: Set two pane names on the layout using the current sub-array entry
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
extern "C" __declspec(noinline) void func_8025629C(CCollepedia* this_) {
    char* str1 = func_802542D0(&this_->field_E8, this_->field_D9, this_->field_D8);
    func_80136B4C(this_->field_38, &lbl_eu_8050C6E8[0x2e2], str1, 0);
    char* str2 = func_80254350(&this_->field_E8, this_->field_D9, this_->field_D8);
    func_80136B4C(this_->field_38, &lbl_eu_8050C6E8[0x2eb], str2, this_->field_4C);
}
#pragma pop
void func_80256314(){}
