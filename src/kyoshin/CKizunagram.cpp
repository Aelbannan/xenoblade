// Auto-scaffolded catalog TU for kyoshin/CKizunagram
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CKizunagram.hpp"
#include "kyoshin/code_80135FDC.hpp"  // layout/anim/font helpers (extern "C" pre-mangled names)
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"

// --- C-linkage / mangled-symbol callees used by the target functions ---
// func_80137444 is C++-linkage (mangled retail), func_80137510 / func_80231848 /
// copyVEC2 are unmangled in retail (extern "C"), and func_80136E84 is referenced
// by its mangled retail identifier (see CSaveLoad for the same pattern).
extern const float lbl_eu_80668828;
extern const float lbl_eu_80668834;
extern const float lbl_eu_80668848;
extern const float lbl_eu_8066885C;
extern const float lbl_eu_80668860;
extern const float lbl_eu_80668864;
extern const float lbl_eu_80668868;
extern const float lbl_eu_8066886C;
extern const float lbl_eu_80668870;
extern u32 lbl_eu_80668838;
extern u32 lbl_eu_8066883C;
extern char lbl_eu_8050CB20[];

void func_80138078(u32 number);

// Same-TU display-state helper consumed by func_8025AB04 / func_8025AB84
// (declaration moved to CKizunagram.hpp C-linkage imports).

// Forward declarations for callees used in func_8025CAB4
// func_8025CE00 is target 5 (takes the display self); the tail calls pass the
// same self pointer. func_8025CE78 / func_8025CF1C are still-unknown stubs.
// noinline: -ipa would fold the empty stub into func_8025CAB4's case-1 tail
// call, turning the retail `b func_8025CE78` into a bare return.
static __declspec(noinline) void func_8025CE78();
extern "C" void func_80257F9C(UnkKizunaSelf57D90* self, u32 a);
extern "C" void func_8025CF1C(void* self) {
    if (*(u8*)((char*)self + 0x8C) != 0) {
        func_80257F9C((UnkKizunaSelf57D90*)((char*)self + 0xAC), 1);
    } else {
        func_80257F9C((UnkKizunaSelf57D90*)((char*)self + 0xAC), 0);
    }
}

// Object with a dispatch byte at 0x3A - drives display state in CKizunagram
struct UnkKizunaDisp {
    u8 _00[0x3A];
    u8 field_0x3A;
};

void func_8025C870() {}


// Toggle both pane entries each frame: walk the +0x08 child to its +0x10
// sub-object, pull its slot-15 layout pane for each id in a 2-word table, and
// repaint the pane matching the given phase byte.
extern "C" __declspec(noinline) void func_80257F9C(UnkKizunaSelf57D90* self, u32 a) {
    u32 paneIds[2] = { lbl_eu_80668838, lbl_eu_8066883C };
    for (u8 i = 0; i < 2; i++) {
        UnkKizunaRes59344* res =
            ((UnkKizunaMid59344*)self->field8)->field10->target(paneIds[i], 1);
        func_80124270((nw4r::lyt::Pane*)res, (u32)(i == a));
    }
}

// ctor: MWCC emits the implicit vptr init (__vt__12CKizunaRadar, reloc-name
// drift vs retail lbl_eu_80537608 — data_value category, EQUIVALENT_MATCH
// route), plus the r4 arg store and zero/float fields.
CKizunaRadar::CKizunaRadar(void* arg) {
    f32 idle = lbl_eu_80668828;
    mArg = arg;
    mField08 = 0;
    mField0C = 0;
    mField10 = 0;
    mField14 = idle;
    mField18 = idle;
}

CKizunaRadar::~CKizunaRadar() {}

CKizunaCur::~CKizunaCur() {}

CKizunaLine::CKizunaLine(u32 arg4, u32 arg8, u8 arg3D) {
    mVtable = lbl_eu_805375F0;
    field4 = arg4;
    field8 = arg8;
    field0C = 0;
    field10 = 0;
    field14 = 0;
    field15 = 1;
    field16 = 0;
    field17 = 0;
    field24 = 0;
    field26 = 0;
    f32 idle = lbl_eu_80668828;
    field28 = idle;
    field2C = idle;
    field30 = idle;
    field34 = 0;
    field36 = 0;
    field38 = idle;
    field3C = 0;
    field3D = arg3D;
    field40 = idle;
}

CKizunaLine::~CKizunaLine() {}

// Release/null a +0x08 child object: if non-null, finalize it (vtable slot 2)
// with a 1 flag, then clear the pointer.
// extern "C" + noinline keeps same-TU callers emitting an unmangled `bl
// func_80257D90` (retail links this symbol unmangled) instead of inlining.
extern "C" __declspec(noinline) void func_80257D90(UnkKizunaSelf57D90* self) {
    // Retail re-tests field8 inside the guard: the duplicated condition CSEs to
    // one cmpwi with two beq's (dead second branch — the btm_sec pattern).
    if (self->field8 != 0) {
        if (self->field8 != 0) {
            self->field8->target2(1);
        }
        self->field8 = 0;
    }
}

// Same shape as func_80257D90 (sibling release/null helper).
extern "C" __declspec(noinline) void func_80257F44(UnkKizunaSelf57D90* self) {
    // Same retail shape as func_80257D90: duplicated guard test CSEs to one
    // cmpwi with two beq's; zeroing inside the outer if.
    if (self->field8 != 0) {
        if (self->field8 != 0) {
            self->field8->target2(1);
        }
        self->field8 = 0;
    }
}

// Build the current-line layout (+0x08) from the shared arc string at +0x27,
// attach its anim transform at +0x0C, then publish both via the layout's
// vtable slots 11 / 14.
extern "C" __declspec(noinline) void func_80257E58(UnkKizunaSelf57E58* self) {
    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        (nw4r::lyt::Layout**)&self->field8, self->field4, lbl_eu_8050CB20 + 0x27);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        (nw4r::lyt::Layout*)self->field8, &self->field0C, self->field4, lbl_eu_8050CB20 + 0x3f);
    self->field8->slot11((u32)self->field0C, 1);
    self->field8->slot14(0);
}

void func_802580CC(UnkKizunaLineState* arg){}

// us-8025b198 (0x8025B198): game-local PSVECAdd kernel — dst[i] = a[i] + b[i], i in 0..2.
// Retail is a pure paired-single body (psq_l/ps_add/psq_st, no frame, arg order r3=dst,
// r4=a, r5=b) that MWCC cannot emit from scalar C++ (no ps_add intrinsic; scalar C++
// yields 9 lfs/fadds/stfs). Isolated Gekko paired-single backend (PLAN.md 17.6).
#if defined(__MWERKS__) && !defined(NONMATCHING)
extern "C" asm void func_80258F5C(register float* dst, register const float* a,
                                  register const float* b) {
    nofralloc
    psq_l f0, 0(r4), 0, 0
    psq_l f1, 0(r5), 0, 0
    ps_add f2, f0, f1
    psq_l f0, 8(r4), 1, 0
    psq_l f1, 8(r5), 1, 0
    psq_st f2, 0(r3), 0, 0
    ps_add f2, f0, f1
    psq_st f2, 8(r3), 1, 0
    blr
}
#else
extern "C" void func_80258F5C(float* dst, const float* a, const float* b) {
    dst[0] = a[0] + b[0];
    dst[1] = a[1] + b[1];
    dst[2] = a[2] + b[2];
}
#endif

// us-8025b1bc (0x8025B1BC): game-local PSVECScale kernel — dst = src * scale (3 floats).
// Retail is a pure paired-single body (psq_l/ps_muls0/psq_st, no frame, arg order r3=dst,
// r4=src, f1=scale). MWCC has no ps_muls0 intrinsic and scalar C++ yields
// lfs/fmuls/stfs. Isolated Gekko paired-single backend (PLAN.md 17.6).
#if defined(__MWERKS__) && !defined(NONMATCHING)
extern "C" asm void func_80258F80(register float* dst, register const float* src,
                                  register float scale) {
    nofralloc
    psq_l f0, 0(r4), 0, 0
    ps_muls0 f2, f0, f1
    psq_l f0, 8(r4), 1, 0
    psq_st f2, 0(r3), 0, 0
    ps_muls0 f2, f0, f1
    psq_st f2, 8(r3), 1, 0
    blr
}
#else
extern "C" void func_80258F80(float* dst, const float* src, float scale) {
    dst[0] = src[0] * scale;
    dst[1] = src[1] * scale;
    dst[2] = src[2] * scale;
}
#endif

void func_80258F9C(){}

extern "C" __declspec(noinline) void func_80259098(UnkKizunaSelf59B18* self) {
    if (self->field0C == 0) return;
    UnkKizunaCtxPane59098* res =
        (UnkKizunaCtxPane59098*)((UnkKizunaLayoutSub57AFC*)self->field0C)
            ->field10->slot15(lbl_eu_8050CB20 + 0x9a, 1);
    const wchar_t* cur = getContextStr((u8*)&res->ctx);
    while (func_801276C8(getContextStrPtr((u8*)&res->ctx), &cur) != 0) {
        const wchar_t* tmp = cur;
        func_801276E0(&tmp, 0);
        const wchar_t* s = func_80127670(&cur);
        nw4r::lyt::Pane* child = res->slot15((const char*)s + 0xbc, 1);
        ((nw4r::lyt::Pane*)res)->RemoveChild(child);
        if (child != 0) {
            if (child != 0) {
                ((UnkKizunaPaneFinalize*)child)->target2(1);
            }
        }
        cur = tmp;
    }
    nw4r::lyt::Pane* root =
        (nw4r::lyt::Pane*)((UnkKizunaLayoutSub57AFC*)self->field0C)
            ->field10->slot15(lbl_eu_8050CB20 + 0xa4, 1);
    root->RemoveChild((nw4r::lyt::Pane*)res);
    if (res != 0) {
        if (res != 0) {
            ((UnkKizunaPaneFinalize*)res)->target2(1);
        }
    }
    if (self->field0C != 0) {
        if (self->field0C != 0) {
            ((UnkKizunaPaneFinalize*)self->field0C)->target2(1);
        }
        self->field0C = 0;
    }
}

// Set an anim child state flag (1), publish it to the object at +0x0C's
// vtable slot 8, then write the shared "idle" float constant at +0x40.
void func_80259228(UnkKizunaSelf59228* self) {
    self->field14 = 1;
    self->field15 = 0;
    self->field0C->target8(self->field10);
    self->field40 = lbl_eu_80668828;
}

// Same shape as func_80259228 but with state flag 3 and a different idle float.
extern "C" __declspec(noinline) void func_80259280(UnkKizunaSelf59228* self) {
    self->field14 = 3;
    self->field15 = 0;
    self->field0C->target8(self->field10);
    self->field40 = lbl_eu_80668848;
}

// True when the anim's current frame has reached (its frame count - 1).
bool func_802592D8(UnkKizunaSelf592D8* self) {
    return self->field10->GetFrame() >= (f32)(self->field10->GetFrameSize() - 1);
}

// Dispatch: walk +0x0C -> +0x10, call vtable slot 15 with (0x8B, 1), then
// return a 2-word struct built from the result's +0x44 / +0x48 words.
// extern "C" + noinline keeps callers (func_8025CAE4/CB50) emitting real
// unmangled bl branches (retail links func_80259344 unmangled) without
// inlining - the CKizunaTalkList pattern for same-TU callees.
extern "C" __declspec(noinline) UnkKizunaPair func_80259344(UnkKizunaSelf59344* self) {
    UnkKizunaRes59344* res = self->field0C->field10->target((int)(lbl_eu_8050CB20 + 0x8b), 1);
    UnkKizunaPair out = *(UnkKizunaPair*)((u8*)res + 0x44);
    return out;
}

extern "C" __declspec(noinline) void func_80259394(UnkKizunaSelf57D90* self, const UnkKizunaVec3* v) {}

extern "C" __declspec(noinline) int func_8025949C(UnkKizunaSelf57D90* self) { return 0; }

extern "C" __declspec(noinline) void func_80259820(UnkKizunaSelf57D90* self) {}

extern "C" void func_80259AF4(char* dest, const char* src) { dest[0] = src[0]; dest[1] = src[1]; dest[2] = src[2]; dest[3] = 0; }

// us-8025bd54 (0x8025BD54): if the shared arc layout's named root pane is
// visible, hide it and re-apply the 5 kizuna pane-name groups for indices
// 1..0x11 to the layout.
void func_80259B18(UnkKizunaSelf59B18* self) {
    nw4r::lyt::Pane* pane = (nw4r::lyt::Pane*)((UnkKizunaLayoutSub57AFC*)self->field0C)
                                ->field10->slot15(lbl_eu_8050CB20 + 0x3f5, 1);
    if (func_801C4648(pane) != 0) {
        func_80124270(pane, 0);
        char buf[0x2C];
        for (u8 i = 1; i <= 0x11; i++) {
            sprintf(buf, lbl_eu_8050CB20 + 0x445, i);
            func_80136B4C(self->field0C, buf, lbl_eu_8050CB20 + 0xd3, 0);
            sprintf(buf, lbl_eu_8050CB20 + 0x454, i);
            func_80136B4C(self->field0C, buf, lbl_eu_8050CB20 + 0xd3, 0);
            sprintf(buf, lbl_eu_8050CB20 + 0x464, i);
            func_80136B4C(self->field0C, buf, lbl_eu_8050CB20 + 0xd3, 0);
            sprintf(buf, lbl_eu_8050CB20 + 0x474, i);
            func_80136B4C(self->field0C, buf, lbl_eu_8050CB20 + 0xd3, 0);
            sprintf(buf, lbl_eu_8050CB20 + 0x484, i);
            func_80136B4C(self->field0C, buf, lbl_eu_8050CB20 + 0xd3, 0);
        }
    }
}

void func_80259C5C(){}

void func_80259D44(){}

// Build the line layout from the shared arc string(+0x4C), then copy the
// animated position at +0x4C of the slot-15 result into the +0x14 Vec2.
void func_80257AFC(UnkKizunaSelfAFC* self) {
    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        &self->field8, self->field4, lbl_eu_8050CB20);
    UnkKizunaLCBRes57AFC* res =
        ((UnkKizunaLayoutSub57AFC*)self->field8)->field10->slot15(lbl_eu_8050CB20 + 0x13, 1);
    copyVEC2(&self->field14[0], res->vec);
}

void func_80259DE8(){}

// Update pass: while the +0x08 child is live, advance the anim at +0x0C unless
// the +0x10 byte suppresses it, then notify the child via vtable slot 14.
void func_80257EE0(UnkKizunaSelf57EE0* self) {
    if (self->field8) {
        if (self->field10 == 0) {
            func_80137444(self->field0C, lbl_eu_80668834);
        }
        self->field8->callSlot14(0);
    }
}

CKizunaCur::CKizunaCur(nw4r::lyt::ArcResourceAccessor* accessor) {
    mVtable = lbl_eu_805375FC;
    mArcResAcc = accessor;
    mField08 = 0;
    mField0C = 0;
    mField10 = 0;
    mField11 = 1;
    mField12 = 0;
}

void func_8025AA38(){}

// retail: if (field26) { field34=1; field36=field26; field38=const }
extern "C" void func_8025AAE0(void* self) {
    u16 v = *(u16*)((char*)self + 0x26);
    if (v != 0) {
        *(u8*)((char*)self + 0x34) = 1;
        *(u16*)((char*)self + 0x36) = v;
        *(float*)((char*)self + 0x38) = lbl_eu_80668828;
    }
}

void func_8025AB04(UnkKizunaSelfAB* self) {
    f32 f = self->field38 + lbl_eu_80668834;
    self->field38 = f;
    if (f >= lbl_eu_80668860) {
        self->field34 = 2;
        self->field38 = lbl_eu_80668828;
        func_8025AC1C(self, 1);
    }
    if (self->field26 == 0) {
        self->field34 = 3;
        func_8025AC1C(self, 0);
    }
}

void func_8025AB84(UnkKizunaSelfAB* self) {
    f32 f = self->field38 + lbl_eu_80668834;
    self->field38 = f;
    if (f >= lbl_eu_80668864) {
        self->field34 = 1;
        self->field38 = lbl_eu_80668828;
        func_8025AC1C(self, 0);
    }
    if (self->field26 == 0) {
        self->field34 = 3;
        func_8025AC1C(self, 0);
    }
}

extern const float lbl_eu_80668828;
void CKizunagram_resetFields(u8* self){
    *(u8*)(self + 0x34) = 0;
    *(u16*)(self + 0x36) = 0;
    *(float*)(self + 0x38) = lbl_eu_80668828;
}

extern "C" __declspec(noinline) void func_8025AC1C(UnkKizunaSelfAB* self, u32 a) {}

void CKizunagram_copyString(unsigned char* dst, const unsigned char* src) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = 0;
}

CKizunaInfo::CKizunaInfo(nw4r::lyt::ArcResourceAccessor* accessor) {
    mArcResAcc = accessor;
    mVtable = lbl_eu_805375E4;
    mField08 = 0;
    mField0C = 0;
    mField10 = 0;
    mField14 = 0;
    mField15 = 1;
    mField16 = 1;
    mField18 = 0;
}

CKizunaInfo::~CKizunaInfo() {}

// us-8025d8ac (0x8025B670): build the kizuna line layout + two anim
// transforms, bind the font to the root pane, publish the 4 font-format
// values and 6 localized labels, then reset the +0x18 counter.
void func_8025B670(UnkKizunaSelfB670* self) {
    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        &self->field8, self->field4, lbl_eu_8050CB20 + 0xb19);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        self->field8, &self->field0C, self->field4, lbl_eu_8050CB20 + 0xb2c);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        self->field8, &self->field10, self->field4, lbl_eu_8050CB20 + 0xb42);
    nw4r::lyt::Pane* rootPane = ((UnkKizunaLayoutView*)self->field8)->field10;
    void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, self->field8);
    u32 fontResult = static_cast<UnkKizunaFontView*>(fontObj)->vf7();
    func_8013676C(rootPane, fontResult);
    u32 val = func_801355BC();
    func_801368C0(self->field8, lbl_eu_8050CB20 + 0xb5d, val);
    func_801368C0(self->field8, lbl_eu_8050CB20 + 0xb6b, val);
    func_801368C0(self->field8, lbl_eu_8050CB20 + 0xb79, val);
    func_801368C0(self->field8, lbl_eu_8050CB20 + 0xb87, val);
    self->field8->UnbindAllAnimation();
    func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xb95,
                  func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0xb), 0);
    func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xb9f,
                  func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0xc), 0);
    func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xba9,
                  func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0xd), 0);
    func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xbb3,
                  func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0xe), 0);
    func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xbbd,
                  func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0xf), 0);
    func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xbc7,
                  func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x10), 0);
    self->field18 = 0xFFFF;
    func_8025BA38((UnkKizunaSelf57D90*)self, 0);
}

// Per-frame display dispatch: while the current-line child (+0x08) is live,
// run the mode-specific handler for the +0x14 mode byte, then notify the
// child via vtable slot 14 (retail: cmpwi chain over modes 1/2/4/5).
extern "C" __declspec(noinline) void func_8025B870(UnkKizunaSelfC21C* self) {
    if (self->field8) {
        switch (self->field14) {
        case 1:
            func_8025C16C(self);
            break;
        case 2:
            func_8025C21C(self);
            break;
        case 4:
            func_8025C298(self);
            break;
        case 5:
            func_8025C348(self);
            break;
        }
        self->field8->target14(0);
    }
}

// Finalize then release/null a +0x08 child: if non-null, finalize it (vtable
// slot 2) with a 1 flag, then clear the pointer.
// The nested identical guards reproduce retail's duplicated test: both beqs
// share the single loaded pointer (MWCC CSEs the load).
extern "C" __declspec(noinline) void func_8025B900(UnkKizunaSelf57D90* self) {
    if (self->field8 != 0) {
        if (self->field8 != 0) {
            self->field8->target2(1);
        }
        self->field8 = 0;
    }
}

// Set display mode 1 for the current-line child: publish the frame value via
// vtable slots 7/11 of the child at +0x08, and clear the animation flags.
extern "C" __declspec(noinline) void func_8025B958(UnkKizunaSelfB958* self) {
    self->field14 = 1;
    self->field15 = 0;
    self->field16 = 0;
    self->field8->slot7(self->field0C);
    self->field8->slot11(self->field0C, 1);
}

// Same shape as func_8025B958 but display mode 4 and value from +0x10.
extern "C" __declspec(noinline) void func_8025B9C8(UnkKizunaSelfB958* self) {
    self->field14 = 4;
    self->field15 = 0;
    self->field16 = 0;
    self->field8->slot7(self->field10);
    self->field8->slot11(self->field10, 1);
}

extern "C" __declspec(noinline) void func_8025BA38(UnkKizunaSelf57D90* self, u16 v) {}

extern "C" __declspec(noinline) void func_8025C16C(UnkKizunaSelfC21C* self) {}

extern "C" __declspec(noinline) void func_8025C21C(UnkKizunaSelfC21C* self) {
    if (func_80137444(self->field10, lbl_eu_80668834) != 0) {
        // anim reached its last frame: switch to mode 3, publish via the child.
        self->field14 = 3;
        self->field15 = 1;
        self->field8->target14(0);
        self->field8->target8((u32)self->field10);
    }
}

extern "C" __declspec(noinline) void func_8025C298(UnkKizunaSelfC21C* self) {}

// func_8025C348 (us-8025e494): kizuna-line update gate. Retail: reads the
// global float lbl_eu_80668834 (sdata), calls func_80137510([self+0xC], f1)
// (declared CArtsInfo.hpp, defined code_80135FDC.cpp — the old comment's
// "no split symbols" blocker is stale); on non-zero return: field14=0,
// field15=1, then [self+8] virtual slot 56 (target14) twice with arg 0,
// then slot 32 (target8) with arg [self+0xC].
extern "C" __declspec(noinline) void func_8025C348(UnkKizunaSelfC21C* self) {
    if (func_80137510((nw4r::lyt::AnimTransform*)self->field0C, lbl_eu_80668834) != 0) {
        self->field14 = 0;
        self->field15 = 1;
        self->field8->target14(0);
        self->field8->target14(0);
        self->field8->target8(self->field0C);
    }
}

CKizunagram::CKizunagram() {}

CKizunagram::~CKizunagram() {}

// Load both bind files into the +0x28 / +0x2C file handles (self is the
// IWorkEvent receiver), flagging each handle after it is read. Locals let MWCC
// reuse the readFile return register for the flag calls (no reload).
void func_8025C510(UnkKizunaSelfC510* self) {
    CFileHandle* h2 = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                            lbl_eu_8050CB20 + 0xcd1,
                                            (IWorkEvent*)self, 0, 0);
    self->field2C = h2;
    CDeviceFile::setHandleFlag2(h2);
    CFileHandle* h1 = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                            lbl_eu_8050CB20 + 0xcf0,
                                            (IWorkEvent*)self, 0, 0);
    self->field28 = h1;
    CDeviceFile::setHandleFlag2(h1);
}

void func_8025C580(){}

void func_8025C61C(){}

// Destruction: release both file handles, clear the anim flag byte, then
// release each child object (0x4C / 0x68 / 0xAC / 0xC0) and the two resource
// accessors, and tear down the two memory regions at +0x08 / +0x18.
void func_8025C6F0(UnkKizunaSelfC6F0* self) {
    func_801390E0(&self->field28);
    func_801390E0(&self->field2C);
    self->field38 = 0;
    func_8025B900(&self->sub4C);
    func_80259098((UnkKizunaSelf59B18*)&self->sub68);
    func_80257F44(&self->subAC);
    func_80257D90(&self->subC0);
    func_80139124(self->field30);
    func_80139124(self->field34);
    self->mRegA.func_8045F778();
    self->mRegB.func_8045F778();
}

unsigned char CKizunagram_getField7E(u8* this_ptr) {
    if (*(unsigned char*)(this_ptr + 0x7e) != 0) {
        return *(unsigned char*)(this_ptr + 0x3b);
    } else {
        return 0;
    }
}

unsigned char CKizunagram_checkFields(u8* arg1)
{
    if (*(unsigned char*)(arg1 + 0x61) == 0)
        return 0;
    if (*(unsigned char*)(arg1 + 0x7d) == 0)
        return 0;
    if (*(unsigned char*)(arg1 + 0xbd) != 0)
        return *(unsigned char*)(arg1 + 0x3c);
    return 0;
}

extern "C" void func_8025C7D0(UnkKizunaSelfC7FC* self) {
    if (self->field39 != 0) return;
    self->field39 = 1;
    self->field3C = 0;
    ((u8*)self)[0x38] = 1;
    func_8025B958(&self->sub4C);
}

void func_8025C7FC(UnkKizunaSelfC7FC* self, int arg4) {
    if (self->field39 == 3) {
        // switch to mode 4, reset the sub-flag, re-publish all children.
        self->field39 = 4;
        self->field3C = 0;
        func_8025B9C8(&self->sub4C);
        func_80259280(&self->sub68);
        func_80257F9C(&self->subAC, 0xff);
        if (arg4 != 0) {
            func_80138078(6);
        }
    }
}


// Build the line layout from the shared arc string at +0x4C, then copy the
// animated position at +0x4C of the slot-15 result into the +0x14 Vec2.
// Each display reset: place a fixed color into the +0x68 sub-object, clear
// its state byte, reset the two line panes, and step the sub-anim.
extern "C" __declspec(noinline) void func_8025C874(UnkKizunaSelfC874* self) {
    UnkKizunaVec3 tmp;
    UnkKizunaVec3 v = *code80135FDC_setVec3(&tmp.x, lbl_eu_80668828,
                                            lbl_eu_80668868, lbl_eu_80668828);
    func_80259394(&self->sub68, &v);
    func_8025949C(&self->sub68);
    self->field3A = 0;
    func_80257F9C(&self->subAC, 0);
    func_8025BA38(&self->sub4C, self->field8E);
    func_80259820(&self->sub68);
}

// Same display reset as func_8025C874 but with the second color constant.
extern "C" __declspec(noinline) void func_8025C904(UnkKizunaSelfC874* self) {
    UnkKizunaVec3 tmp;
    UnkKizunaVec3 v = *code80135FDC_setVec3(&tmp.x, lbl_eu_80668828,
                                            lbl_eu_8066886C, lbl_eu_80668828);
    func_80259394(&self->sub68, &v);
    func_8025949C(&self->sub68);
    self->field3A = 0;
    func_80257F9C(&self->subAC, 0);
    func_8025BA38(&self->sub4C, self->field8E);
    func_80259820(&self->sub68);
}

// Same display reset as func_8025C874 but with the color constants swapped
// (retail loads f2 first then fmr f3, so the source order is x/y/z with the
// shared constant in y/z).
extern "C" __declspec(noinline) void func_8025C994(UnkKizunaSelfC874* self) {
    UnkKizunaVec3 tmp;
    UnkKizunaVec3 v = *code80135FDC_setVec3(&tmp.x, lbl_eu_8066886C,
                                            lbl_eu_80668828, lbl_eu_80668828);
    func_80259394(&self->sub68, &v);
    func_8025949C(&self->sub68);
    self->field3A = 0;
    func_80257F9C(&self->subAC, 0);
    func_8025BA38(&self->sub4C, self->field8E);
    func_80259820(&self->sub68);
}

void func_8025CA24(){}

// Dispatch on display-state byte at +0x3A; each case is a tail call.
void func_8025CAB4(UnkKizunaDisp* self) {
    switch (self->field_0x3A) {
    case 0:
        return func_8025CE00((UnkKizunaSelfCE00*)self);
    case 1:
        return func_8025CE78();
    case 2:
        return func_8025CF1C(self);
    }
}

// Pulse the anim at +0x78 to frame 1.0, notify the child object (+0x74, via
// the embedded func_80259344 sub at +0x68) on vtable slot 14, rebuild the
// two-word pair, and copy it into the +0xB4 object's child.
void func_8025CAE4(UnkKizunaSelfCAE4* self) {
    func_80137444(self->field78, lbl_eu_80668834);
    ((UnkKizunaObjSlot14*)self->sub.field0C)->callSlot14(0);
    UnkKizunaPair p = func_80259344(&self->sub);
    func_80231848(self->fieldB4->field10, &p);
}

// Same shape as func_8025CAE4 but the anim step uses func_80137510 instead.
void func_8025CB50(UnkKizunaSelfCAE4* self) {
    func_80137510(self->field78, lbl_eu_80668834);
    ((UnkKizunaObjSlot14*)self->sub.field0C)->callSlot14(0);
    UnkKizunaPair p = func_80259344(&self->sub);
    func_80231848(self->fieldB4->field10, &p);
}

int CKizunagram_stub(void* self) { return 0; }

struct CKizunagramState {
    u8 _00[0x8C];
    u8 field8C;
};

extern "C" u8 func_8025CBC4(CKizunagramState* self) {
    return self->field8C;
}

void func_8025CBCC(){}

// retail: lfs f0,const; li r0,0; stb 0x34; sth 0x36; stfs 0x38
extern "C" void func_8025AC04(void* self) {
    *(u8*)((char*)self + 0x34) = 0;
    *(u16*)((char*)self + 0x36) = 0;
    *(float*)((char*)self + 0x38) = lbl_eu_80668828;
}

// retail: lbz 0xdd; xori; subic; subfe; stb - toggle (x^1)!=0 under -O4,s
extern "C" void func_8025CC70(void* self) {
    *(u8*)((char*)self + 0xDD) = (u8)((*(u8*)((char*)self + 0xDD) ^ 1) != 0);
}

// retail: if (field_62) { field_39 = 2; tail func_80259228(self+0x68) }
extern "C" void func_8025CC88(void* self) {
    if (*(u8*)((char*)self + 0x62) != 0) {
        *(u8*)((char*)self + 0x39) = 2;
        func_80259228((UnkKizunaSelf59228*)((char*)self + 0x68));
    }
}

void func_8025CCA8(){}

void CKizunagram_setField39(u8* ptr) {
    if (ptr[0x62] != 0) {
        ptr[0x39] = 5;
    }
}

void CKizunagram_resetState(u8* b) {
    if (!b[0x61]) return;
    if (!b[0x7d]) return;
    b[0x39] = 0;
    b[0x3c] = 1;
    b[0x38] = 0;
}

void func_8025CD40(){}

// If the sub-state at +0x68 reports active, scale the stored int bit patterns
// at +0x80..0x88 down by the constant and publish the results. The u32 words
// are routed through a stack array so MWCC emits the retail lwz/stw/lfs
// bit-cast round-trips for +0x88 without consuming its result.
extern "C" __declspec(noinline) void func_8025CE00(UnkKizunaSelfCE00* self) {
    if (func_8025949C(&self->sub68) != 0) {
        u32 raw[3];
        raw[1] = self->field84;
        raw[0] = self->field80;
        raw[2] = self->field88;   // dead store (never read) — retail keeps it
        f32 denom = lbl_eu_80668870;
        f32 f0 = *(f32*)&raw[1] / denom;
        f32 f1 = *(f32*)&raw[0] / denom;
        self->field48 = denom;
        self->field3A = 1;
        self->field44 = f0;
        self->field40 = f1;
    }
}


__declspec(noinline) void func_8025CE78(){}

void func_8025CF1C(){}

// us-8025f08c (0x8025CF40): when both +0x30/+0x34 pointers are present, build a
// fresh CKizunaLine, copy it into the +0x68 line state (field-by-field,
// including the +0x18..+0x20 pad words the ctor leaves uninitialized), then
// rebuild the line display and re-fetch the two cur/line panes via slot 15.
void func_8025CF40(UnkKizunaSelfCF40* self) {
    if (self->field30 != 0 && self->field34 != 0) {
        self->field3B = 1;
        CKizunaLine line(self->field30, self->field34, self->fieldDE);
        self->lineState.field4 = line.field4;
        self->lineState.field8 = line.field8;
        self->lineState.field0C = line.field0C;
        self->lineState.field10 = line.field10;
        self->lineState.field14 = line.field14;
        self->lineState.field15 = line.field15;
        self->lineState.field16 = line.field16;
        self->lineState.field17 = line.field17;
        self->lineState.field18 = line.field18;
        self->lineState.field1C = line.field1C;
        self->lineState.field20 = line.field20;
        self->lineState.field24 = line.field24;
        self->lineState.field26 = line.field26;
        self->lineState.field28 = line.field28;
        self->lineState.field2C = line.field2C;
        self->lineState.field30 = line.field30;
        self->lineState.field34 = line.field34;
        self->lineState.field36 = line.field36;
        self->lineState.field38 = line.field38;
        self->lineState.field3C = line.field3C;
        self->lineState.field3D = line.field3D;
        self->lineState.field40 = line.field40;
        func_802580CC(&self->lineState);
        self->fieldCC =
            (u32)((UnkKizunaMid59344*)self->lineState.field0C)->field10->target(
                (int)(lbl_eu_8050CB20 + 0xa4), 1);
        self->fieldD0 =
            (u32)((UnkKizunaMid59344*)self->lineState.field0C)->field10->target(
                (int)(lbl_eu_8050CB20 + 0x8b), 1);
    }
}

void CKizunagram::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// sinit_8025D304: init the 14-color kizuna line palette via func_801C4B60.
void sinit_8025D304() {
    func_801C4B60(&lbl_eu_806647E8, 0x21, 0x1c, 0x15, 0x0);
    func_801C4B60(&lbl_eu_806647F0, 0xb5, 0xb1, 0xab, 0xff);
    func_801C4B60(&lbl_eu_806647F8, 0x21, 0x1e, 0x15, 0x0);
    func_801C4B60(&lbl_eu_80664800, 0xb5, 0xb1, 0xab, 0xff);
    func_801C4B60(&lbl_eu_80664808, 0x17, 0x21, 0x15, 0x0);
    func_801C4B60(&lbl_eu_80664810, 0xb3, 0xaf, 0x97, 0xff);
    func_801C4B60(&lbl_eu_80664818, 0x1c, 0x24, 0x2d, 0x0);
    func_801C4B60(&lbl_eu_80664820, 0xb5, 0xb1, 0xab, 0xff);
    func_801C4B60(&lbl_eu_80664828, 0x1c, 0x20, 0x31, 0x0);
    func_801C4B60(&lbl_eu_80664830, 0xb5, 0xb1, 0xab, 0xff);
    func_801C4B60(&lbl_eu_80664838, 0x1c, 0x2c, 0x1a, 0x0);
    func_801C4B60(&lbl_eu_80664840, 0xe5, 0xea, 0xde, 0xff);
    func_801C4B60(&lbl_eu_80664848, 0x21, 0x1e, 0x15, 0x0);
    func_801C4B60(&lbl_eu_80664850, 0xb5, 0xb1, 0xab, 0xff);
}

// Position the kizuna line panes: scale the +0x14 Vec2 by (const - the +0x10
// pane's scale) into the "line" pane's size, then scale the +0x0C pane's
// translate by the ratio of the two reference-pane size deltas into the line
// pane's translate. Finally pulse the layout anim.
void func_80257B6C(UnkKizunaSelf57B6C* self) {
    if (self->field8 != 0) {
        if (self->field10 != 0) {
            nw4r::math::VEC2 sc = self->field10->GetScale();
            f32 sx = lbl_eu_8066882C - sc.x;
            f32 sy = lbl_eu_8066882C - sc.y;
            f32 tmp[2];
            func_80127BC4(tmp, self->field14);
            tmp[0] *= sx;
            tmp[1] *= sy;
            nw4r::lyt::Pane* pane = self->field8->GetRootPane()->FindPaneByName(
                lbl_eu_8050CB20 + 0x13, 1);
            copyVEC2(const_cast<f32*>(&pane->GetSize().width), tmp);
        }
        if (self->field0C != 0) {
            const char* strbase = lbl_eu_8050CB20;
            nw4r::lyt::Pane* c = self->field0C;
            nw4r::math::VEC3 tr = c->GetTranslate();
            tr.x *= lbl_eu_80668830;
            tr.y *= lbl_eu_80668830;
            nw4r::lyt::Pane* paneA = self->field8->GetRootPane()->FindPaneByName(
                strbase + 0x13, 1);
            f32 sa[2];
            func_80127BC4(sa, const_cast<f32*>(&paneA->GetSize().width));
            nw4r::lyt::Pane* paneB = self->field8->GetRootPane()->FindPaneByName(
                strbase + 0x1d, 1);
            f32 sb[2];
            func_80127BC4(sb, const_cast<f32*>(&paneB->GetSize().width));
            f32 sc2[2];
            func_80127BC4(sc2, const_cast<f32*>(&c->GetSize().width));
            sb[0] -= sa[0];
            sb[1] -= sa[1];
            f32 rx = sb[0] / sc2[0];
            f32 ry = sb[1] / sc2[1];
            tr.x *= rx;
            tr.y *= ry;
            nw4r::lyt::Pane* paneC = self->field8->GetRootPane()->FindPaneByName(
                strbase + 0x13, 1);
            copyVEC3(const_cast<f32*>(&paneC->GetTranslate().x), &tr.x);
        }
        self->field8->Animate(0);
    }
}

extern "C" void func_8025A11C() {}
