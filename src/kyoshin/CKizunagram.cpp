// Auto-scaffolded catalog TU for kyoshin/CKizunagram
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CKizunagram.hpp"
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
extern const u32 lbl_eu_80668838;
extern const u32 lbl_eu_8066883C;
extern char lbl_eu_8050CB20[];

u32 func_80137444(nw4r::lyt::AnimTransform* anim, float frame);
void func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
    nw4r::lyt::Layout** ppLayout,
    nw4r::lyt::ArcResourceAccessor* accessor,
    const char* name);
void func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
    nw4r::lyt::Layout* layout,
    nw4r::lyt::AnimTransform** ppAnimTrans,
    nw4r::lyt::ArcResourceAccessor* accessor,
    char* name);
void func_801390E0(CFileHandle** ppHandle);
void func_80139124(nw4r::lyt::ArcResourceAccessor* accessor);
void func_80138078(u32 number);

// Same-TU display-state helper consumed by func_8025AB04 / func_8025AB84
// (declaration moved to CKizunagram.hpp C-linkage imports).

// Forward declarations for callees used in func_8025CAB4
// func_8025CE00 is target 5 (takes the display self); the tail calls pass the
// same self pointer. func_8025CE78 / func_8025CF1C are still-unknown stubs.
static void func_8025CE78();
static void func_8025CF1C();

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
    u32 paneIds[2];
    paneIds[0] = lbl_eu_80668838;
    paneIds[1] = lbl_eu_8066883C;
    for (u8 i = 0; i < 2; i++) {
        UnkKizunaRes59344* res =
            ((UnkKizunaMid59344*)self->field8)->field10->target((int)paneIds[i], 1);
        func_80124270((nw4r::lyt::Pane*)res, (u32)(a == i));
    }
}

CKizunaRadar::CKizunaRadar() {}

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
    if (self->field8 != 0) {
        self->field8->target2(1);
    }
    self->field8 = 0;
}

// Same shape as func_80257D90 (sibling release/null helper).
extern "C" __declspec(noinline) void func_80257F44(UnkKizunaSelf57D90* self) {
    if (self->field8 != 0) {
        self->field8->target2(1);
    }
    self->field8 = 0;
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

void func_802580CC(){}

void func_80258F5C(){}

// retail uses psq_l/ps_muls0 paired-single: dst = src * scalar (auto-vectorized by MWCC -O4,s)
extern "C" void func_80258F80(float* dst, const float* src, float f1) {
    dst[0] = src[0] * f1;
    dst[1] = src[1] * f1;
    dst[2] = src[2] * f1;
}

void func_80258F9C(){}

extern "C" __declspec(noinline) void func_80259098(UnkKizunaSelf57D90* self) {}

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
    UnkKizunaRes59344* res = self->field0C->field10->target(0x8b, 1);
    UnkKizunaPair out = {res->field44, res->field48};
    return out;
}

void func_80259394(){}

extern "C" __declspec(noinline) int func_8025949C(UnkKizunaSelf57D90* self) { return 0; }

void func_80259820(){}

extern "C" void func_80259AF4(char* dest, const char* src) { dest[0] = src[0]; dest[1] = src[1]; dest[2] = src[2]; dest[3] = 0; }

void func_80259B18(){}

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

void func_8025AAE0(){}

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

void func_8025B670(){}

void func_8025B870(){}

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

void func_8025BA38(){}

void func_8025C16C(){}

void func_8025C21C(UnkKizunaSelfC21C* self) {
    if (func_80137444(self->field10, lbl_eu_80668834) != 0) {
        // anim reached its last frame: switch to mode 3, publish via the child.
        self->field14 = 3;
        self->field15 = 1;
        self->field8->target14(0);
        self->field8->target8((u32)self->field10);
    }
}

void func_8025C298(){}

void func_8025C348(){}

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
    func_80259098(&self->sub68);
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

void func_8025C7D0(){}

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


void func_8025C874(){}

void func_8025C904(){}

void func_8025C994(){}

void func_8025CA24(){}

// Dispatch on display-state byte at +0x3A; each branch is a tail call.
void func_8025CAB4(UnkKizunaDisp* self) {
    if (self->field_0x3A == 0) {
        return func_8025CE00((UnkKizunaSelfCE00*)self);
    }
    if (self->field_0x3A == 1) {
        return func_8025CE78();
    }
    if (self->field_0x3A == 2) {
        return func_8025CF1C();
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
        raw[0] = self->field80;
        raw[1] = self->field84;
        raw[2] = self->field88;
        f32 denom = lbl_eu_80668870;
        // field48 carries the constant; +0x88 never feeds a published field.
        *(u32*)&raw[2] = (u32)denom - (u32)denom; // keep raw write alive
        self->field48 = denom;
        self->field3A = 1;
        self->field44 = *(f32*)&raw[1] / denom;
        self->field40 = *(f32*)&raw[0] / denom;
    }
}


void func_8025CE78(){}

void func_8025CF1C(){}

void func_8025CF40(){}

void CKizunagram::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_8025D304(){}

extern "C" void func_80257B6C() {}
extern "C" void func_8025A11C() {}
