// Auto-scaffolded catalog TU for kyoshin/CCollepedia
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CCollepedia.hpp"
#include "nw4r/lyt.h"

// Extern C-linkage helpers from other TUs
extern "C" {
    void __dl__FPv(void*);
    u32 CSysWin_isReady(u8*);
    u32 CSysWin_getUnk34(u8*);
    int CSysWin_isActive(u8*);
    char lbl_eu_8050C6E8[];
    void* lbl_eu_806640A8;
    char* func_80136190(char*, char*, u32);
    char* func_8013639C(void*, char*, u16);
    void func_80138078__FUl(u32);
    u32 func_80136254(char*, char*, u16);
    void func_8013E2E0(u32, u32, u32, u32, u32, u32, u32, u32, u32);
    void func_80137924(void*, void*, void*, void*);
    extern char* lbl_eu_806647DC;
    extern u16 lbl_eu_8050C6A0[];
}

int sprintf(char*, const char*, ...);

// Forward declarations for stubs defined at end of file
void func_80254040(u8*);
void func_8025406C(u8*);
u32 func_802540DC(u8*);
u32 func_80254144(u8*);
void func_80255F98(CCollepedia*);
void func_8025629C(CCollepedia*);
void func_80256314(CCollepedia*);
void func_80255210(CCollepedia*);

void __ct__CCLPCur(){}

void __dt__802531C4(){}

void func_80253204(){}

void func_802532FC(){}

void func_80253360(){}

void func_8025338C(){}

void func_802533F4(){}

void CCollepedia_reset(CCollepedia* obj, int val) {
    obj->field_0 = val;
    obj->field_4 = 0;
    obj->field_8 = 0;
    obj->field_c = 0;
    obj->field_d = 1;
    obj->field_e = 0;
}

extern "C" void* __dt__802534B0(void* self, int dealloc_flag) {
    void* result = self;
    if (result != nullptr && dealloc_flag > 0) {
        __dl__FPv(result);
    }
    return result;
}

void func_802534F0(){}

void func_80253794(){}

void func_802537EC(){}

void func_8025385C(){}

void func_80253888(){}

void func_802538B0(){}

void func_80253904(){}

// Target 1: Formats a string, finds two panes by name, calls func_80137924
void func_80253970(CCollepedia* this_, LayoutContainer* container, int arg3) {
    char buf[0x28];
    sprintf(buf, &lbl_eu_8050C6E8[0x14f], arg3 + 1);
    func_80137924(this_,
        container->mpLayout->GetRootPane()->FindPaneByName(buf, true),
        container->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050C6E8[0x15c], true),
        container->mpLayout->GetRootPane());
}

void func_80253A14(){}

void func_80253A60(){}

void func_80253AB0(){}

extern "C" void* __dt__80253AFC(void* self, int dealloc_flag) {
    void* result = self;
    if (result != nullptr && dealloc_flag > 0) {
        __dl__FPv(result);
    }
    return result;
}

void func_80253B3C(){}

bool func_80253EE8(u8* this_, u32 arg1, u32 arg2) {
    if (arg1 >= 6) return false;
    if (arg2 >= 5) return false;
    s8 idx = (s8)this_[1];
    u8 val = *(this_ + idx * 0x140 + arg1 * 0x34 + arg2 * 0xA + 0x16);
    return val == 2;
}

void func_80253F3C(){}

void CCollepedia_decrementWrap(unsigned char* obj) {
    obj[1] = obj[1] - 1;
    if ((signed char)obj[1] < 0) {
        obj[1] = obj[0] - 1;
    }
}

char* func_80254094(u8* self) {
    s8 idx = (s8)self[1];
    u8* base = self + idx * 0x140;
    u8 val = base[4];
    if (val == 0x1a) {
        return func_80136190(&lbl_eu_8050C6E8[0xA2], &lbl_eu_8050C6E8[0xAE], 0x14);
    } else {
        return func_8013639C(lbl_eu_806640A8, &lbl_eu_8050C6E8[0xAE], val);
    }
}

unsigned char CCollepedia_getFieldAtIdx(u8* thisPtr) {
    s8 idx = (s8)thisPtr[1];
    u8* base = thisPtr + idx * 320;
    return base[4];
}

void func_802540F4(){}

unsigned char CCollepedia_getFieldOffset(const unsigned char* ptr) {
    int idx = (signed char)ptr[1];
    return ptr[5 + idx * 0x140];
}

void func_8025415C(){}

void func_8025418C(){}

void func_802541BC(){}

u16 CCollepedia_getField12(u8* p1, u32 arg2, u32 arg3) {
    if (arg2 >= 6) return 0;
    if (arg3 >= 5) return 0;
    s8 idx = (s8)p1[1];
    u8* ptr = p1 + idx * 0x140 + arg2 * 0x34 + arg3 * 0xa;
    return reinterpret_cast<CCollepediaEntry*>(ptr)[1].field_08;
}

void func_8025424C(){}

void func_802542B8(){}

void func_802542D0(){}

void func_80254350(){}

void func_8025440C(){}

void func_80254484(){}

void func_8025449C(){}

void func_802545C0(){}

void __ct__CCollepedia(){}

void func_8025492C(){}

void func_80254A20(){}

void func_80254B64(){}

void func_80254C04(){}

u8 func_80254D0C(CCollepedia* self) {
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

void func_80254D8C(){}

void func_80254E64(){}

void func_80254F2C(){}

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

void func_802550B4(){}

// Target 2: Decrements field_D9, wrapping, then calls helper functions
void func_8025516C(CCollepedia* this_) {
    if (this_->field_49 != 3) return;
    if (this_->field_28FA != 0) return;
    if (CSysWin_getUnk34(&this_->field_9C)) return;

    func_80254040(&this_->field_E8);
    u32 count = func_80254144(&this_->field_E8);

    if ((s8)this_->field_D9 >= (s32)(count & 0xFF)) {
        this_->field_D9 = (u8)(func_80254144(&this_->field_E8) - 1);
        func_80256314(this_);
    }

    func_80255F98(this_);
    func_8025629C(this_);
    func_80138078__FUl(10);
}

// Target 3: Decrements field_D9, wrapping (calls func_8025406C instead of func_80254040)
void func_80255210(CCollepedia* this_) {
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

void func_802552B4(){}

void func_802553AC(){}

void func_80255688(void* self) {
    CCollepediaFull* full = static_cast<CCollepediaFull*>(self);
    func_80253EE8((u8*)self + 0xE8, full->field_D9, full->field_D8);
}

u8 func_80255698(CCollepedia* self) {
    if (CSysWin_getUnk34(&self->field_9C)) {
        return 1;
    }
    return self->field_28FA;
}

void func_802556DC(){}

void func_80255748(){}

void func_802557E0(){}

void func_80255894(){}

void func_8025592C(){}

void func_80255984(){}

void func_802559DC(){}

// Target 4: If CSysWin is active, set state, look up table entry, format string, call func_8013E2E0
void func_80255AB4(CCollepedia* this_) {
    if (!CSysWin_isActive(&this_->field_9C)) return;

    this_->field_49 = 12;

    u32 idx = func_802540DC(&this_->field_E8);
    // Table is u16 array at lbl_eu_8050C6A0; index u8 then *2 for lhzx byte offset
    u32 idxMask = idx & 0xFF;
    u16 tableVal = lbl_eu_8050C6A0[idxMask];

    u32 result = func_80136254(lbl_eu_806647DC, &lbl_eu_8050C6E8[0x1f9],
        (u16)((u16)(tableVal + 1) + (s8)this_->field_D9));

    u32 zero = 0;
    func_8013E2E0((u16)result, 0, 0, 0, 0, 0, 0, 0, zero);
}

void func_80255B60(){}

void func_80255C28(){}

void func_80255CC0(){}

void func_80255D3C(){}

void func_80255E90(){}

void func_8025641C(){}

// Stubs for functions called by targets (no params to prevent optimization)
void func_80254040(){}
void func_8025406C(){}
u32 func_802540DC(){ return 0; }
u32 func_80254144(){ return 0; }
void func_80255F98(){}
void func_8025629C(){}
void func_80256314(){}
