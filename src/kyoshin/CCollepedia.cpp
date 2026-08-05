// Auto-scaffolded catalog TU for kyoshin/CCollepedia
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CCollepedia.hpp"

// Extern C-linkage helpers from other TUs
extern "C" {
    void __dl__FPv(u8*);
    u32 CSysWin_isReady(CSysWinFull*);
    u32 CSysWin_getUnk34(CSysWinFull*);
    int CSysWin_isActive(CSysWinFull*);
    char lbl_eu_8050C6E8[];
    u16 lbl_eu_8050C6A0[30];
    u32 lbl_eu_806640A8;
    u32 lbl_eu_806647DC;
    char* func_80136190(char*, char*, u32);
    char* func_8013639C(u32, char*, u16);
    int func_80137444__FPQ34nw4r3lyt13AnimTransformf(nw4r::lyt::AnimTransform*, float);
    int func_80137510(nw4r::lyt::AnimTransform*, float);
    u16 func_80136254(u32, char*, int);
    void func_8013E2E0(u16, int, int, int, int, int, int, int, int);
    void func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
    void func_801D20B0(u8*, nw4r::lyt::DrawInfo*);
    void func_8022B7C8(CSysWinFull*, nw4r::lyt::DrawInfo*);
    void func_80138078__FUl(u32);
    void func_80256314(CCollepedia*);
    void func_80255F98(CCollepedia*);
    void func_8025629C(CCollepedia*);
    void func_80254040(u8*);
    void func_8025406C(u8*);
    unsigned char func_80254144(u8*);
}

void __ct__CCLPCur(){}

void __dt__802531C4(){}

void func_80253204(){}

void func_802532FC(){}

void func_80253360(u8*, nw4r::lyt::DrawInfo*){}

void func_8025338C(){}

// Target 1: us-80255630
// Unbind/bind animations, then animate the layout
void func_802533F4(CCollepedia* self) {
    self->mpLayout1->UnbindAnimation(self->mpAnim2);
    self->mpLayout1->BindAnimation(self->mpAnim1);
    self->mpLayout1->SetAnimationEnable(self->mpAnim1, true);
    self->mpAnim1->SetFrame(0.0f);
    self->mpLayout1->Animate();
}

void CCollepedia_reset(CCollepedia* obj, int val) {
    obj->field_0 = val;
    obj->field_4 = 0;
    obj->mpLayout1 = nullptr;
    obj->mpAnim1 = nullptr;
}

CCollepedia::~CCollepedia() {}

void func_802534F0(){}

void func_80253794(){}

void func_802537EC(){}

void func_8025385C(){}

void func_80253888(){}

void func_802538B0(){}

void func_80253904(){}

void func_80253970(){}

void func_80253A14(){}

void func_80253A60(){}

void func_80253AB0(){}

void __dt__80253AFC(){}

void func_80253B3C(){}

bool func_80253EE8(u8* this_, u32 arg1, u32 arg2) {
    if (arg1 >= 6) return false;
    if (arg2 >= 5) return false;
    s8 idx = (s8)this_[1];
    u8 val = *(this_ + idx * 0x140 + arg1 * 0x34 + arg2 * 0xA + 0x16);
    return val == 2;
}

void func_80253F3C(){}

// Stub for func_802540DC - not inlined to match retail call pattern
extern "C" u8 __attribute__((noinline)) func_802540DC(u8* self) {
    s8 idx = (s8)self[1];
    return *(u8*)(self + idx * 0x140 + 4);
}

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

// Target 5: us-80256da0
// Draw the collection UI elements if the state is active
void func_80254B64(CCollepedia* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->field_48 != 0 && self->field_49 != 0) {
        func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(self->mpLayout2, drawInfo, 0, 1);
        func_80253360(&self->field_6C, drawInfo);
        if (self->mpLayout3 != nullptr) {
            func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(self->mpLayout3, drawInfo, 0, 1);
        }
        func_801D20B0(&self->field_54, drawInfo);
        func_8022B7C8((CSysWinFull*)&self->field_9C, drawInfo);
        func_801D20B0(&self->field_84, drawInfo);
    }
}

void func_80254C04(){}

u8 func_80254D0C(CCollepedia* self) {
    if (CSysWin_isReady((CSysWinFull*)&self->field_9C)) {
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

void func_80255000(){}

void func_802550B4(){}

// Target 1: us-802573a8
// Called when field_49 == 3 (specific state), field_28FA == 0 (not busy),
// and CSysWin's unk34 == 0 (not active). Increments the sub-array index,
// then conditionally decrements field_D9 based on the index comparison.
void func_8025516C(CCollepedia* self) {
    // Guard conditions: must be in state 3, not busy, and CSysWin inactive
    if (self->field_49 != 3) return;
    if (self->field_28FA != 0) return;
    if (CSysWin_getUnk34((CSysWinFull*)&self->field_9C) != 0) return;

    // Advance the sub-array index
    func_80254040(&self->field_E8);

    // If field_D9 (signed) >= current index, wrap around by decrementing
    if ((s8)self->field_D9 >= (u8)func_80254144(&self->field_E8)) {
        self->field_D9 = (u8)(func_80254144(&self->field_E8) - 1);
        func_80256314(self);
    }

    func_80255F98(self);
    func_8025629C(self);
    func_80138078__FUl(10);
}

// Sub-array header at +0xE8: byte[0] = max value, byte[1] = current index
// Increments the index and wraps around when it reaches the max value
__declspec(noinline) void func_80254040(u8* self) {
    self[1]++;
    if ((s8)self[1] >= self[0]) {
        self[1] = 0;
    }
}

// Returns the byte at offset 5 of the entry at index (s8)self[1] (stride 0x140)
__declspec(noinline) unsigned char func_80254144(u8* self) {
    return (self + (s8)self[1] * 320)[5];
}

// Decrements the sub-array index and wraps to (max-1) when it goes negative
__declspec(noinline) void func_8025406C(u8* self) {
    self[1] = self[1] - 1;
    if ((s8)self[1] < 0) {
        self[1] = self[0] - 1;
    }
}

// Called when field_49 == 3 (specific state), field_28FA == 0 (not busy),
// and CSysWin's unk34 == 0 (not active). Decrements the sub-array index,
// then conditionally decrements field_D9 based on the index comparison.
void func_80255210(CCollepedia* self) {
    // Guard conditions: must be in state 3, not busy, and CSysWin inactive
    if (self->field_49 != 3) return;
    if (self->field_28FA != 0) return;
    if (CSysWin_getUnk34((CSysWinFull*)&self->field_9C) != 0) return;

    // Step the sub-array index backward (decrement with wrap)
    func_8025406C(&self->field_E8);

    // If field_D9 (signed) >= current index, wrap around by decrementing
    if ((s8)self->field_D9 >= (u8)func_80254144(&self->field_E8)) {
        self->field_D9 = (u8)(func_80254144(&self->field_E8) - 1);
        func_80256314(self);
    }

    func_80255F98(self);
    func_8025629C(self);
    func_80138078__FUl(0xA);
}

void func_802552B4(){}

void func_802553AC(){}

void func_80255688(u8* self) {
    CCollepediaFull* full = reinterpret_cast<CCollepediaFull*>(self);
    func_80253EE8((u8*)self + 0xE8, full->field_D9, full->field_D8);
}

u8 func_80255698(CCollepedia* self) {
    if (CSysWin_getUnk34((CSysWinFull*)&self->field_9C)) {
        return 1;
    }
    return self->field_28FA;
}

void func_802556DC(){}

// Target 2: us-80257984
// Check if animation is done, then update animation bindings and state
void func_80255748(CCollepedia* self) {
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(self->mpAnim3, 0.0f) != 0) {
        self->mpLayout2->SetAnimationEnable(self->mpAnim5, false);
        self->mpLayout2->SetAnimationEnable(self->mpAnim3, false);
        self->mpLayout2->SetAnimationEnable(self->mpAnim4, true);
        self->field_49 = 2;
    }
}

void func_802557E0(){}

// Target 3: us-80257ad0
// Similar to target 2, different animation slot and state value
void func_80255894(CCollepedia* self) {
    if (func_80137510(self->mpAnim4, 0.0f) != 0) {
        self->mpLayout2->SetAnimationEnable(self->mpAnim4, false);
        self->mpLayout2->SetAnimationEnable(self->mpAnim5, false);
        self->mpLayout2->SetAnimationEnable(self->mpAnim3, true);
        self->field_49 = 5;
    }
}

void func_8025592C(){}

void func_80255984(){}

void func_802559DC(){}

// Target 1: us-80257cf0
// If CSysWin is active, set the state flag and play a selection sound whose
// message index is derived from the collection's current category + entry.
void func_80255AB4(CCollepedia* self) {
    if (CSysWin_isActive((CSysWinFull*)&self->field_9C)) {
        self->field_49 = 0xC;
        u16 val = lbl_eu_8050C6A0[func_802540DC(&self->field_E8) & 0xFF];
        u16 r = func_80136254(lbl_eu_806647DC, &lbl_eu_8050C6E8[0x1F9],
                              (u16)(val + 1) + (s8)self->field_D9);
        func_8013E2E0(r, 0, 0, 0, 0, 0, 0, 0, 0);
    }
}

void func_80255B60(){}

// Target 4: us-80257e64
// Check if CSysWin is active, then set state and create a UI sound effect
void func_80255C28(CCollepedia* self) {
    if (CSysWin_isActive((CSysWinFull*)&self->field_9C)) {
        self->field_49 = 0x10;
        u16 val = lbl_eu_8050C6A0[func_802540DC(&self->field_E8) & 0xFF];
        u16 r = func_80136254(lbl_eu_806647DC, &lbl_eu_8050C6E8[0x1F9], val);
        func_8013E2E0(r, 0, 0, 0, 0, 0, 0, 0, 0);
    }
}

void func_80255CC0(){}

void func_80255D3C(){}

void func_80255E90(){}

void func_80255F98(){}

void func_8025629C(){}

void func_80256314(){}

void func_8025641C(){}
