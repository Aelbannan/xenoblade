// Translation unit: monolib/src/scn/code_8047D2AC
//
// Two related scene classes sharing an identical 0x28-byte state block:
//   UnkClass_8047D2AC -- route/path follower state machine ("walker")
//   UnkClass_8047E064 -- setup/init wrapper
//
// All methods call the global scene manager at lbl_eu_80658560
// (UnkClass_8047E110) and the node-pool manager at lbl_eu_80658540
// (UnkClass_8047CD0C).
//
// Retail main.dol is stripped; symbols.txt annotates every method as Fv
// even though the real code passes register parameters.  Functions that
// take register parameters are therefore defined with extern "C" linkage
// using the exact retail mangled name (PLAN.md §17.6 relocation-name
// pattern).  Float constants use `extern const f32 lbl_eu_*` so the load
// schedules like a pooled literal AND the relocation keeps the retail name
// (MWCC_REFERENCE §1b).

#include <harness_catalog.h>
#include <types.h>
#include <revolution/MTX.h>

// ------------------------------------------------------------------
// Shared state layout (0x28 bytes)
// ------------------------------------------------------------------

struct UnkClass_8047D2AC {
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
// Externals: global managers + constants (retail linker names, §17.6)
// ------------------------------------------------------------------

extern "C" {

extern u8 lbl_eu_80658560[]; // UnkClass_8047E110 scene manager (bss 0xA8)
extern u8 lbl_eu_80658540[]; // UnkClass_8047CD0C node pool (bss)

extern const f32 lbl_eu_8066A878; // 0.5f
extern const f32 lbl_eu_8066A87C; // 1.0f
extern const f32 lbl_eu_8066A880; // 0.0f
extern const f32 lbl_eu_8066A884; // 0.01f
extern const f64 lbl_eu_8066A888; // 2^52 (int->float magic)

// UnkClass_8047CD0C
void* func_8047D178__17UnkClass_8047CD0CFv(void* self);

// UnkClass_8047E110 -- scene manager methods (retail Fv annotations).
// Signatures recovered from caller register setup.
void func_80481014__17UnkClass_8047E110Fv(void* self, f32 a, f32 b, void* v, f32 c);
s32 func_8047E6C4__17UnkClass_8047E110Fv(void* self, void* out, void* a, u16 b, u16 c, void* d, f32 f);
s32 func_8047E62C__17UnkClass_8047E110Fv(void* self, void* out, void* a, void* b, f32 f);
void func_8047F214__17UnkClass_8047E110Fv(void* self, void* out, void* pos, u32 a, u16 node);
s32 func_8047F658__17UnkClass_8047E110Fv(void* self, void* out);
s32 func_8047FE48__17UnkClass_8047E110Fv(void* self, void* state, u32 a, u32 b, u32 c, u32 d);
s32 func_8048020C__17UnkClass_8047E110Fv(void* self, void* out, void* arr, u32 a, u32 b, u32 c);
s32 func_804804DC__17UnkClass_8047E110Fv(void* self, void* out, void* arr, u32 a, u32 b, u32 c);
s32 func_804808A0__17UnkClass_8047E110Fv(void* self, u32 a, f32 f, void* pos, u32 b, u16 node);
s32 func_80480EF0__17UnkClass_8047E110Fv(void* self, u16 node);
s32 func_80480F48__17UnkClass_8047E110Fv(void* self, u32 a, u16 b, u16 c);
void func_804814DC__17UnkClass_8047E110Fv(void* self, f32 f, void* a, void* b);
s32 func_80481790__17UnkClass_8047E110Fv(void* self, u32 a);
void func_8048163C__17UnkClass_8047E110Fv(void* self, void* a);
void func_8048169C__17UnkClass_8047E110Fv(void* self, void* a, void* b);
s32 func_804819AC__17UnkClass_8047E110Fv(void* self, u32 a);
s32 func_804819C4__17UnkClass_8047E110Fv(void* self, u32 a);

// In-TU walker methods (extern "C" -- retail Fv mangling, take registers)
s32 func_8047DF54__17UnkClass_8047D2ACFv(UnkClass_8047D2AC* self, u32* arg1, void* arg2);

} // extern "C"

// ------------------------------------------------------------------
// UnkClass_8047E064 methods
// ------------------------------------------------------------------

// func_8047E100 -- clear/reset (0x10)
void UnkClass_8047E064::func_8047E100() {
    mField0 = 0;
    mFlags = 0;
}

// func_8047E0B8 -- initialize defaults (0x48)
void UnkClass_8047E064::func_8047E0B8() {
    mFlags |= 1;
    mField8 = 0;
    mFieldA = 0;
    mFieldC = 0;
    mFieldE = 0;
    mField20 = 1;
    mField12 = 0x50;
    mField4 = lbl_eu_8066A880;
    mField22 = 4;
    mField24 = 0;
}

// func_8047E064 -- attach to node pool (0x54)
extern "C" void func_8047E064__17UnkClass_8047E064Fv(UnkClass_8047E064* self, u16 val) {
    self->mField0 = (u32)func_8047D178__17UnkClass_8047CD0CFv(lbl_eu_80658540);
    self->mFlags |= 2;
    self->mField1E = val;
}

// ------------------------------------------------------------------
// UnkClass_8047D2AC walker methods
// ------------------------------------------------------------------

// func_8047DF54 -- advance one step using the E6C4/E62C segment solvers (0x110)
extern "C" s32 func_8047DF54__17UnkClass_8047D2ACFv(UnkClass_8047D2AC* self, u32* arg1, void* arg2) {
    if (!(self->mFlags & 1)) return 0;
    if (!(self->mFlags & 2)) return 0;

    f32 ga, gb;
    func_80481014__17UnkClass_8047E110Fv(lbl_eu_80658560, ga, gb, arg2, self->mSpeed);

    s32 result = 0;
    u32 localA;
    if (self->mFlags & 4) {
        if (func_8047E6C4__17UnkClass_8047E110Fv(lbl_eu_80658560, &localA, arg1,
                                                  self->mField18, self->mField14, arg2,
                                                  lbl_eu_8066A878)) {
            result = 1;
            self->mField18 = (u16)localA;
            self->mField14 = (u16)*arg1;
        }
    }
    if (!result) {
        if (func_8047E62C__17UnkClass_8047E110Fv(lbl_eu_80658560, &localA, arg1, arg2,
                                                  lbl_eu_8066A87C)) {
            result = 1;
            self->mField18 = (u16)localA;
            self->mField14 = (u16)*arg1;
        }
    }
    return result;
}

// func_8047DE14 -- thin wrapper: run DF54 into a scratch word (0x28)
extern "C" s32 func_8047DE14__17UnkClass_8047D2ACFv(UnkClass_8047D2AC* self, void* arg) {
    u32 local;
    return func_8047DF54__17UnkClass_8047D2ACFv(self, &local, arg);
}

// func_8047DE3C -- advance using E6C4/E62C, storing to field16/field1A (0x118)
extern "C" s32 func_8047DE3C__17UnkClass_8047D2ACFv(UnkClass_8047D2AC* self, void* arg) {
    if (!(self->mFlags & 1)) return 0;
    if (!(self->mFlags & 2)) return 0;

    f32 ga, gb;
    func_80481014__17UnkClass_8047E110Fv(lbl_eu_80658560, ga, gb, arg, self->mSpeed);

    s32 result = 0;
    u32 localB, localA;
    if (self->mFlags & 4) {
        if (func_8047E6C4__17UnkClass_8047E110Fv(lbl_eu_80658560, &localA, &localB,
                                                  self->mField18, self->mField14, arg,
                                                  lbl_eu_8066A878)) {
            result = 1;
            self->mField1A = (u16)localA;
            self->mField16 = (u16)localB;
        }
    }
    if (!result) {
        if (func_8047E62C__17UnkClass_8047E110Fv(lbl_eu_80658560, &localA, &localB, arg,
                                                  lbl_eu_8066A87C)) {
            result = 1;
            self->mField1A = (u16)localA;
            self->mField16 = (u16)localB;
        }
    }
    return result;
}

// func_8047DC8C -- solve + query scene manager, copy result vector (0xC0)
extern "C" s32 func_8047DC8C__17UnkClass_8047D2ACFv(UnkClass_8047D2AC* self, u32* out,
                                                     void* arg3, void* arg4,
                                                     f32 fx, f32 fy, f32 fz) {
    u32 local;
    if (!func_8047DF54__17UnkClass_8047D2ACFv(self, &local, arg3)) return 0;

    func_804814DC__17UnkClass_8047E110Fv(lbl_eu_80658560, fx, arg3, arg4);
    if (!func_80481790__17UnkClass_8047E110Fv(lbl_eu_80658560, local)) return 0;

    const u32* mgr = (const u32*)lbl_eu_80658560;
    out[0] = mgr[0x6c / 4];
    out[1] = mgr[0x70 / 4];
    out[2] = mgr[0x74 / 4];
    return 1;
}

// func_8047DD4C -- solve + branch on arg3 to 8169C/819C4 or 8163C/81790 (0xC8)
extern "C" s32 func_8047DD4C__17UnkClass_8047D2ACFv(UnkClass_8047D2AC* self, void* arg1,
                                                     void* arg2, u32 arg3) {
    u32 local;
    if (!func_8047DF54__17UnkClass_8047D2ACFv(self, &local, arg1)) return 0;
    if (func_804819AC__17UnkClass_8047E110Fv(lbl_eu_80658560, local)) return 0;
    if (arg3) {
        func_8048169C__17UnkClass_8047E110Fv(lbl_eu_80658560, arg1, arg2);
        return func_804819C4__17UnkClass_8047E110Fv(lbl_eu_80658560, local);
    } else {
        func_8048163C__17UnkClass_8047E110Fv(lbl_eu_80658560, arg2);
        return func_80481790__17UnkClass_8047E110Fv(lbl_eu_80658560, local);
    }
}

// func_8047D2AC -- full walker tick (0x9E0) -- stub for now
extern "C" s32 func_8047D2AC__17UnkClass_8047D2ACFv(UnkClass_8047D2AC* self, void* outDir,
                                                     void* posA, void* posB) {
    return 2;
}
