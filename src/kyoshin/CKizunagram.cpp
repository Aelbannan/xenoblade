// Auto-scaffolded catalog TU for kyoshin/CKizunagram
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CKizunagram.hpp"

// --- C-linkage / mangled-symbol callees used by the target functions ---
// func_80137444 is C++-linkage (mangled retail), func_80137510 / func_80231848 /
// copyVEC2 are unmangled in retail (extern "C"), and func_80136E84 is referenced
// by its mangled retail identifier (see CSaveLoad for the same pattern).
extern const float lbl_eu_80668828;
extern const float lbl_eu_80668834;
extern const float lbl_eu_80668848;
extern char lbl_eu_8050CB20[];

u32 func_80137444(nw4r::lyt::AnimTransform* anim, float frame);
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform* anim, float frame);
extern "C" void func_80231848(UnkKizunaFunc31848Obj* self, const UnkKizunaPair* src);
extern "C" void copyVEC2(float* dst, const float* src);
void func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
    nw4r::lyt::Layout** ppLayout,
    nw4r::lyt::ArcResourceAccessor* accessor,
    const char* name);

// Forward declarations for callees used in func_8025CAB4
static void func_8025CE00();
static void func_8025CE78();
static void func_8025CF1C();

// Object with a dispatch byte at 0x3A — drives display state in CKizunagram
struct UnkKizunaDisp {
    u8 _00[0x3A];
    u8 field_0x3A;
};

void func_8025C870() {}










void func_80257F9C(){}

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
void func_80257D90(UnkKizunaSelf57D90* self) {
    if (self->field8 != 0) {
        self->field8->target2(1);
    }
    self->field8 = 0;
}

// Same shape as func_80257D90 (sibling release/null helper).
void func_80257F44(UnkKizunaSelf57D90* self) {
    if (self->field8 != 0) {
        self->field8->target2(1);
    }
    self->field8 = 0;
}

void func_802580CC(){}

void func_80258F5C(){}

void func_80258F80(){}

void func_80258F9C(){}

void func_80259098(){}

// Set an anim child state flag (1), publish it to the object at +0x0C's
// vtable slot 8, then write the shared "idle" float constant at +0x40.
void func_80259228(UnkKizunaSelf59228* self) {
    self->field14 = 1;
    self->field15 = 0;
    self->field0C->target8(self->field10);
    self->field40 = lbl_eu_80668828;
}

// Same shape as func_80259228 but with state flag 3 and a different idle float.
void func_80259280(UnkKizunaSelf59228* self) {
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
UnkKizunaPair func_80259344(UnkKizunaSelf59344* self) {
    UnkKizunaRes59344* res = self->field0C->field10->target(0x8b, 1);
    UnkKizunaPair out = {res->field44, res->field48};
    return out;
}

void func_80259394(){}

void func_8025949C(){}

void func_80259820(){}

void CKizunagram_copyShortString(char* dest, const char* src) { dest[0] = src[0]; dest[1] = src[1]; dest[2] = src[2]; dest[3] = 0; }

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

void func_8025AB04(){}

void func_8025AB84(){}

extern const float lbl_eu_80668828;
void CKizunagram_resetFields(void* self){
    *(u8*)((u8*)self + 0x34) = 0;
    *(u16*)((u8*)self + 0x36) = 0;
    *(float*)((u8*)self + 0x38) = lbl_eu_80668828;
}

void func_8025AC1C(){}

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
void func_8025B900(UnkKizunaSelf57D90* self) {
    if (self->field8 != 0) {
        if (self->field8 != 0) {
            self->field8->target2(1);
        }
        self->field8 = 0;
    }
}

void func_8025B958(){}

void func_8025B9C8(){}

void func_8025BA38(){}

void func_8025C16C(){}

void func_8025C21C(){}

void func_8025C298(){}

void func_8025C348(){}

CKizunagram::CKizunagram() {}

CKizunagram::~CKizunagram() {}

void func_8025C510(){}

void func_8025C580(){}

void func_8025C61C(){}

void func_8025C6F0(){}

unsigned char CKizunagram_getField7E(void* this_ptr) {
    if (*(unsigned char*)((unsigned char*)this_ptr + 0x7e) != 0) {
        return *(unsigned char*)((unsigned char*)this_ptr + 0x3b);
    } else {
        return 0;
    }
}

unsigned char CKizunagram_checkFields(void* arg1)
{
    if (*(unsigned char*)((char*)arg1 + 0x61) == 0)
        return 0;
    if (*(unsigned char*)((char*)arg1 + 0x7d) == 0)
        return 0;
    if (*(unsigned char*)((char*)arg1 + 0xbd) != 0)
        return *(unsigned char*)((char*)arg1 + 0x3c);
    return 0;
}

void func_8025C7D0(){}

void func_8025C7FC(){}


void func_8025C874(){}

void func_8025C904(){}

void func_8025C994(){}

void func_8025CA24(){}

// Dispatch on display-state byte at +0x3A; each branch is a tail call.
void func_8025CAB4(UnkKizunaDisp* self) {
    if (self->field_0x3A == 0) {
        return func_8025CE00();
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

u8 CKizunagram_getField8C(void* self) {
    return ((CKizunagramState*)self)->field8C;
}

void func_8025CBCC(){}

void func_8025CC70(){}

void func_8025CC88(){}

void func_8025CCA8(){}

void CKizunagram_setField39(void* arg) {
    unsigned char* ptr = (unsigned char*)arg;
    if (ptr[0x62] != 0) {
        ptr[0x39] = 5;
    }
}

void CKizunagram_resetState(void* self) {
    uint8_t* b = static_cast<uint8_t*>(self);
    if (!b[0x61]) return;
    if (!b[0x7d]) return;
    b[0x39] = 0;
    b[0x3c] = 1;
    b[0x38] = 0;
}

void func_8025CD40(){}

void func_8025CE00(){}

void func_8025CE78(){}

void func_8025CF1C(){}

void func_8025CF40(){}

void CKizunagram::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_8025D304(){}
