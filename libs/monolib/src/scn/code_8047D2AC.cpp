// Translation unit: monolib/src/scn/code_8047D2AC
//
// Two related scene classes:
//   UnkClass_8047D2AC -- route/path follower state machine ("walker")
//   UnkClass_8047E064 -- setup/init wrapper for the same state block
//
// Both share an identical field layout (see below).  All methods call
// into the global scene manager at lbl_eu_80658560 (UnkClass_8047E110)
// and the node-pool manager at lbl_eu_80658540 (UnkClass_8047CD0C).
//
// Retail symbols are stripped; symbols.txt annotates every method as
// Fv.  Functions that take register parameters are therefore defined
// with extern "C" linkage using the exact retail mangled name (PLAN.md
// §17.6 relocation-name pattern).

#include <harness_catalog.h>
#include <types.h>
#include <revolution/MTX.h>

// ------------------------------------------------------------------
// Shared state layout (0x28 bytes)
// ------------------------------------------------------------------

struct UnkClass_8047D2AC {
    void func_8047D2AC() const;
    void func_8047DC8C() const;
    void func_8047DD4C() const;
    void func_8047DE14() const;
    void func_8047DE3C() const;
    void func_8047DF54() const;

    u16* mNodes;    // 0x00 - node index array
    f32 mSpeed;     // 0x04
    u16 mNodeIdx;   // 0x08
    u16 mSegIdx;    // 0x0A
    u16 mSegCount;  // 0x0C
    u16 mSegPos;    // 0x0E
    u16 mFlags;     // 0x10
    u16 mField12;   // 0x12
    u16 mField14;   // 0x14
    u16 mField16;   // 0x16
    u16 mField18;   // 0x18
    u16 mField1A;   // 0x1A
    u16 mField1C;   // 0x1C
    u16 mField1E;   // 0x1E
    u16 mStep;      // 0x20
    u16 mLimit;     // 0x22
    u16 mStuck;     // 0x24
};

struct UnkClass_8047E064 {
    void func_8047E0B8();
    void func_8047E100();

    u32 mField0;    // 0x00
    f32 mField4;    // 0x04
    u16 mField8;    // 0x08
    u16 mFieldA;    // 0x0A
    u16 mFieldC;    // 0x0C
    u16 mFieldE;    // 0x0E
    u16 mFlags;     // 0x10
    u16 mField12;   // 0x12
    u16 mField14;   // 0x14
    u16 mField16;   // 0x16
    u16 mField18;   // 0x18
    u16 mField1A;   // 0x1A
    u16 mField1C;   // 0x1C
    u16 mField1E;   // 0x1E
    u16 mField20;   // 0x20
    u16 mField22;   // 0x22
    u16 mField24;   // 0x24
};

// ------------------------------------------------------------------
// Externals: global managers (retail linker names, §17.6)
// ------------------------------------------------------------------

extern "C" {

extern u8 lbl_eu_80658560[]; // UnkClass_8047E110 scene manager (bss 0xA8)
extern u8 lbl_eu_80658540[]; // UnkClass_8047CD0C node pool (bss)

// Shared sdata2 float constants (retail linker names, §17.6)
extern f32 lbl_eu_8066A878; // 0.5f
extern f32 lbl_eu_8066A87C; // 1.0f
extern f32 lbl_eu_8066A880; // 0.0f
extern f32 lbl_eu_8066A884; // 0.01f

// UnkClass_8047CD0C
void* func_8047D178__17UnkClass_8047CD0CFv(void* self);

// UnkClass_8047E110 -- scene manager methods (retail Fv annotations)
void func_80481014__17UnkClass_8047E110Fv(void* self, f32 a, f32 b, f32 c, void* p);
s32 func_8047E6C4__17UnkClass_8047E110Fv(void* self, u16* outA, u16* outB, f32 f, u16 a, u16 b, void* node);
s32 func_8047E62C__17UnkClass_8047E110Fv(void* self, u16* outA, u16* outB, f32 f, void* node);
void func_8047F214__17UnkClass_8047E110Fv(void* self, void* out, void* pos, u32 a, u16 node);
s32 func_8047F658__17UnkClass_8047E110Fv(void* self, void* out);
s32 func_8047FE48__17UnkClass_8047E110Fv(void* self, void* state, u32 a, u32 b, u32 c, u32 d);
s32 func_8048020C__17UnkClass_8047E110Fv(void* self, void* out, u16* arr, u32 a, u32 b, u32 c);
s32 func_804804DC__17UnkClass_8047E110Fv(void* self, void* out, u16* arr, u32 a, u32 b, u32 c);
s32 func_804808A0__17UnkClass_8047E110Fv(void* self, u32 a, f32 f, void* pos, u32 b, u16 node);
s32 func_80480EF0__17UnkClass_8047E110Fv(void* self, u16 node);
s32 func_80480F48__17UnkClass_8047E110Fv(void* self, u32 a, u16 b, u16 c);
void func_804814DC__17UnkClass_8047E110Fv(void* self, f32 f, void* a, void* b);
s32 func_80481790__17UnkClass_8047E110Fv(void* self, u32 a);
void func_8048163C__17UnkClass_8047E110Fv(void* self, void* a);
void func_8048169C__17UnkClass_8047E110Fv(void* self, void* a, void* b);
s32 func_804819AC__17UnkClass_8047E110Fv(void* self, u32 a);
void func_804819C4__17UnkClass_8047E110Fv(void* self, u32 a);

} // extern "C"

// ------------------------------------------------------------------
// UnkClass_8047E064 methods
// ------------------------------------------------------------------

// func_8047E100 -- clear/reset (0x10 bytes)
void UnkClass_8047E064::func_8047E100() {
    mField0 = 0;
    mFlags = 0;
}

// func_8047E0B8 -- initialize defaults (0x48 bytes)
void UnkClass_8047E064::func_8047E0B8() {
    mFlags |= 1;
    mField8 = 0;
    mFieldA = 0;
    mFieldC = 0;
    mFieldE = 0;
    mField20 = 1;
    mField12 = 0x50;
    mField4 = 0.0f;
    mField22 = 4;
    mField24 = 0;
}

// func_8047E064 -- attach to node pool (0x54 bytes)
extern "C" void func_8047E064__17UnkClass_8047E064Fv(UnkClass_8047E064* self, u16 val) {
    self->mField0 = (u32)func_8047D178__17UnkClass_8047CD0CFv(lbl_eu_80658540);
    self->mFlags |= 2;
    self->mField1E = val;
}

// ------------------------------------------------------------------
// UnkClass_8047D2AC methods
// ------------------------------------------------------------------

void UnkClass_8047D2AC::func_8047D2AC() const {}

void UnkClass_8047D2AC::func_8047DC8C() const {}

void UnkClass_8047D2AC::func_8047DD4C() const {}

void UnkClass_8047D2AC::func_8047DE14() const {}

void UnkClass_8047D2AC::func_8047DE3C() const {}

void UnkClass_8047D2AC::func_8047DF54() const {}
