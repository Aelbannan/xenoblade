// Decompiled unit kyoshin/cf/CtrlMoveNpc.
//
// CCtrlMoveNpc - NPC movement controller. Derived from CCtrlMoveBase (base
// occupies 0x00..0x4B; secondary vtable at 0x48 overwritten by the derived
// ctor). Own fields begin at 0x4C; mStateFunc (0x4C) is a ptmf, null by
// default and set to a real state function by func_8019F6E8.
//
// Layout / types are defined here (this TU has no unit header); struct layouts
// recovered from the retail assembly in the session brief.

#include "kyoshin/harness_catalog.hpp"
#include <monolib/math/CVec3.hpp>
#include <revolution/MTX.h>
#include "kyoshin/cf/CfGameManager.hpp"

// ---------------------------------------------------------------------------
// Types (unit-private)
// ---------------------------------------------------------------------------

// MWCC pointer-to-member-function (12 bytes). Layout matches Runtime/ptmf.c:
//   this_delta @+0, v_offset @+4, f_data @+8.
struct PTMF {
    int this_delta;
    int v_offset;
    int f_data;
};

namespace cf {

// Sub-object reached through CCtrlMoveNpc::mBaseData->field_0x28. Only the two
// virtual slots used by func_8019F6E8 are named; the rest are padding.
class CNpcMoveSub {
public:
    // Naming: retail getPos() sits at vtable slot 43 (0xac) and getF78() at
    // slot 78 (0x138); the real object has 2 implicit leading vtable entries
    // (dtor + vbase), so only 41 explicit placeholders precede getPos().
    virtual void _v0();
    virtual void _v1();
    virtual void _v2();
    virtual void _v3();
    virtual void _v4();
    virtual void _v5();
    virtual void _v6();
    virtual void _v7();
    virtual void _v8();
    virtual void _v9();
    virtual void _v10();
    virtual void _v11();
    virtual void _v12();
    virtual void _v13();
    virtual void _v14();
    virtual void _v15();
    virtual void _v16();
    virtual void _v17();
    virtual void _v18();
    virtual void _v19();
    virtual void _v20();
    virtual void _v21();
    virtual void _v22();
    virtual void _v23();
    virtual void _v24();
    virtual void _v25();
    virtual void _v26();
    virtual void _v27();
    virtual void _v28();
    virtual void _v29();
    virtual void _v30();
    virtual void _v31();
    virtual void _v32();
    virtual void _v33();
    virtual void _v34();
    virtual void _v35();
    virtual void _v36();
    virtual void _v37();
    virtual void _v38();
    virtual void _v39();
    virtual void _v40();
    virtual ml::CVec3* getPos();   // slot 43 (0xac)
    virtual void _v44();
    virtual void _v45();
    virtual void _v46();
    virtual void _v47();
    virtual void _v48();
    virtual void _v49();
    virtual void _v50();
    virtual void _v51();
    virtual void _v52();
    virtual void _v53();
    virtual void _v54();
    virtual void _v55();
    virtual void _v56();
    virtual void _v57();
    virtual void _v58();
    virtual void _v59();
    virtual void _v60();
    virtual void _v61();
    virtual void _v62();
    virtual void _v63();
    virtual void _v64();
    virtual void _v65();
    virtual void _v66();
    virtual void _v67();
    virtual void _v68();
    virtual void _v69();
    virtual void _v70();
    virtual void _v71();
    virtual void _v72();
    virtual void _v73();
    virtual void _v74();
    virtual void _v75();
    virtual void _v76();
    virtual void _v77();
    virtual float* getF78();   // slot 78 (0x138)
};

// Layout of CCtrlMoveNpc::mBaseData (0x34). Only fields used here are named.
struct CNpcBaseData {
    u8 _00[0x14];
    f32 field_0x14;      // 0x14
    u8 _18[0x28 - 0x18];
    CNpcMoveSub* field_0x28;  // 0x28
};

class CCtrlMoveNpc {
public:
    CCtrlMoveNpc();

    // --- base class (CCtrlMoveBase) region 0x00..0x4B ---
    char mBase00[0x18];     // 0x00 primary vtable + base fields
    ml::CVec3 mVec18;       // 0x18
    char mBase24[0x0C];     // 0x24..0x2F
    void* mBase30;          // 0x30
    void* mBaseData;        // 0x34 data ptr (CNpcBaseData)
    f32 mBase38;            // 0x38
    f32 mBase3C;            // 0x3C
    u16 mBase40;            // 0x40
    u16 mBase42;            // 0x42
    u8  mBase44;            // 0x44
    u8  mBase45;            // 0x45
    char mBase46[2];        // 0x46..0x47
    void* mBaseVtable2;     // 0x48 secondary vtable (overwritten here)

    // --- CCtrlMoveNpc own fields ---
    int (CCtrlMoveNpc::*mStateFunc)();  // 0x4C state dispatch ptmf (12 bytes)
    ml::CVec3 mField58;     // 0x58 movement target (raw-copied from vec)
    u32 mField64;           // 0x64
    f32 mField68;           // 0x68
    f32 mField6C;           // 0x6C
    f32 mField70;           // 0x70
    f32 mField74;           // 0x74
    u32 mField78;           // 0x78
};

} // namespace cf

using cf::CCtrlMoveNpc;
using cf::CNpcBaseData;
using cf::CNpcMoveSub;

// ---------------------------------------------------------------------------
// Imports (retail symbol names - global scope, unmangled)
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

extern "C" {

// Base class ctor + ptmf runtime.
void __ct__80088904(CCtrlMoveNpc* self);
long __ptmf_test(PTMF* ptmf);
void __ptmf_scall(...);

// initial state-function ptmf constant.
extern int (CCtrlMoveNpc::*const lbl_eu_80532DA8)();

// bdat helpers / task helpers / movement helper (unmangled global symbols).
void* func_8003AA34();
u32 func_8003B41C(void* bdat);
u32 func_8003B1EC(void* bdat);
u32 getBdatStringColumnValue(void* data, const char* col, s32 idx);
void func_80462D04__8CTaskLODFv(s8 v);
void func_80462D5C__8CTaskLODFv(u8 v);
void* func_804BC9EC__Fv(void);
void func_804BCC30(void*, s8 v);
void func_804BCC3C(void*, u8 v);
void func_80089990(CCtrlMoveNpc* self);
f32 FrSqrt__Q24nw4r4mathFf(f32 x);

// Movement base helpers / npc move helpers (unmangled global symbols).
void func_80088974(CCtrlMoveNpc* self, const ml::CVec3* a, const ml::CVec3* b,
                   int c, int d);
void func_80089694(CCtrlMoveNpc* self, const ml::CVec3* a, f32 f);
void func_8008962C(CCtrlMoveNpc* self);
void func_80093618(CNpcBaseData* data, f32 f);
int  func_800A5038(const ml::CVec3* sub, const ml::CVec3* v, f32 f1, f32 f2);
void Warning__Q24nw4r2dbFPCciPCce(const char* file, int line, const char* fmt, ...);

// cf::CfGameManager::func_80082354 is declared non-static void in
// CfGameManager.hpp, but the retail call site passes no instance and uses the
// return value, so the header's declaration is wrong here. Declare the mangled
// symbol directly.
u16 func_80082354__Q22cf13CfGameManagerFv();

}

extern "C" {

// vtable; take its address, so declare as an array.
extern const char lbl_eu_80532DB4[];

// Warning() file / format rodata strings.
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

// ptr member-function null constant (3 words).
extern int (CCtrlMoveNpc::*const __ptmf_null)();

// sdata2 float literals referenced by func_8019F6E8.
extern const f32 lbl_eu_80667C58;
extern const f32 lbl_eu_80667C5C;
extern const f32 lbl_eu_80667C60;
extern const f32 lbl_eu_80667C64;
extern const f32 lbl_eu_80667C68;
extern const f32 lbl_eu_8066A208;   // ml::epsilon
// Extra sdata2 literals used by func_8019F93C.
extern const f32 lbl_eu_80667C6C;
extern const f32 lbl_eu_80667C70;
extern const f32 lbl_eu_80667C74;
extern const f32 lbl_eu_80667C78;
extern const f32 lbl_eu_8066A210;   // pi/2

// bdat column string table + row bitmap + other globals.
extern const char lbl_eu_80503D30[];
extern u32 lbl_eu_805757E0[];
extern void* lbl_eu_806640B4;   // active table bdat data
extern u32 lbl_eu_80663E24;     // global flags
extern u32 lbl_eu_80663D90;     // global value copied into mField78

}

// ---------------------------------------------------------------------------
// Target 1: __ct__cf_CtrlMoveNpc - constructor
// ---------------------------------------------------------------------------
namespace cf {
CCtrlMoveNpc::CCtrlMoveNpc() {
    __ct__80088904(this);
    this->mBaseVtable2 = (void*)lbl_eu_80532DB4;
    this->mStateFunc = __ptmf_null;
}
}

// ---------------------------------------------------------------------------
// Target 2: func_8019F8E0 - dispatch the state ptmf if set; return "was set".
// ---------------------------------------------------------------------------
namespace cf {
int func_8019F8E0(CCtrlMoveNpc* self) {
    if (self->mStateFunc) {
        (self->*self->mStateFunc)();
    }
    return self->mStateFunc != 0 ? 1 : 0;
}
}

// ---------------------------------------------------------------------------
// Target 5: func_8019F6E8 - pick a movement target, initialise move fields and
// set the state fn. Approached-fraction is clamped into [0, max].
// ---------------------------------------------------------------------------
namespace cf {
void func_8019F6E8(CCtrlMoveNpc* self, const ml::CVec3* vec, f32 scale, f32 paramB) {
    self->mField64 = 4;
    self->mField58 = *vec;
    self->mField68 = scale;
    self->mField6C = paramB;

    // The base data pointer is re-loaded for each access in the retail.
    self->mField74 =
        *(((CNpcBaseData*)self->mBaseData)->field_0x28->getF78());
    self->mField78 = lbl_eu_80663D90;
    self->mField70 = lbl_eu_80667C58;
    ((CNpcBaseData*)self->mBaseData)->field_0x14 = lbl_eu_80667C5C;

    // PS vector subtraction: delta = target - current position.
    ml::CVec3* pos = ((CNpcBaseData*)self->mBaseData)->field_0x28->getPos();
    ml::CVec3 diff = *vec - *pos;
    float len2 = diff.x * diff.x + diff.z * diff.z;

    if (len2 < lbl_eu_80667C5C) {
        Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
    }
    // Normalise the horizontal distance, guarding the epsilon case.
    float sq;
    if (len2 > lbl_eu_80667C5C) {
        sq = len2 * FrSqrt__Q24nw4r4mathFf(len2);
    } else {
        sq = lbl_eu_80667C5C;
    }

    float t = lbl_eu_80667C64 * sq / scale + lbl_eu_80667C60;
    if (t > lbl_eu_8066A208) {
        float v = t / self->mField74;
        self->mField70 = v;
        if (v < lbl_eu_80667C58) {
            v = lbl_eu_80667C58;
        } else if (v > lbl_eu_80667C68) {
            v = lbl_eu_80667C68;
        }
        self->mField70 = v;
    }

    func_80089990(self);
    self->mVec18 = ml::CVec3::zero;
    self->mStateFunc = lbl_eu_80532DA8;
}
}

// ---------------------------------------------------------------------------
// Target 4: func_8019FB54 - range check an Npc table row against row/value
// columns. Returns whether the row is "usable" for the current situation.
// Defined at global scope: the retail symbol is unmangled.
// ---------------------------------------------------------------------------
int func_8019FB54(u32 idx, const char* p1, const char* p2, const char* p3,
                  const char* p4, const char* p5, const char* p6, const char* p7) {
    void* bdat = (void*)lbl_eu_806640B4;
    // Column values live in address-taken slots (full-word stores after each
    // call); the u8/u16 casts at each use site lower to lbz/lhz (retail
    // shape, cf. CfGimmickSaveOff ctor).
    u32 v0 = getBdatStringColumnValue(bdat, lbl_eu_80503D30, idx);
    u32 f = lbl_eu_80663E24;
    if ((f & (0x02000000u | 0x400u)) && *(u8*)&v0 != 0) {
        return 0;
    }

    u16 range0 = (u16)cf::CfGameManager::func_800822F4();
    u32 v20 = getBdatStringColumnValue(bdat, p4, idx);
    u32 v1c = getBdatStringColumnValue(bdat, p5, idx);
    u32 v18 = getBdatStringColumnValue(bdat, p6, idx);
    u32 v14 = getBdatStringColumnValue(bdat, p7, idx);
    int flag1 = 1;
    if (*(u16*)&v1c != 0 && (u32)*(u16*)&v20 <= (u32)range0 &&
        (u32)range0 <= (u32)*(u16*)&v1c) flag1 = 0;
    if (*(u16*)&v14 != 0 && (u32)*(u16*)&v18 <= (u32)range0 &&
        (u32)range0 <= (u32)*(u16*)&v14) flag1 = 0;

    u32 v10 = getBdatStringColumnValue(bdat, p1, idx);
    int flag2 = 1;
    if (*(u16*)&v10 != 0) {
        u16 range1 = func_80082354__Q22cf13CfGameManagerFv();
        u32 v0c = getBdatStringColumnValue(bdat, p2, idx);
        u32 v08 = getBdatStringColumnValue(bdat, p3, idx);
        if ((u32)*(u8*)&v0c <= (u32)range1 && (u32)range1 <= (u32)*(u8*)&v08)
            flag2 = 0;

        if (*(u16*)&v1c != 0 || *(u16*)&v14 != 0) {
            return (flag1 != 0 || flag2 != 0);
        }
        return flag2;
    }
    return flag1;
}

// ---------------------------------------------------------------------------
// Target 3: func_8019FD2C - walk the table rows, toggling a per-row bit in the
// bitmap and notifying LOD when the usable-flag flips.
// Defined at global scope: the retail symbol is unmangled.
// ---------------------------------------------------------------------------
void func_8019FD2C() {
    func_8003AA34();
    void* bdat = (void*)lbl_eu_806640B4;
    cf::CfGameManager::func_80083298();

    u32* bitmap;
    const char* str;
    s32 base = (s32)func_8003B41C(bdat);
    s32 count = (s32)func_8003B1EC(bdat);
    bitmap = lbl_eu_805757E0;
    str = lbl_eu_80503D30;
    s32 end = base + count;

    for (s32 idx = base; idx < end; idx++) {
        // Byte-extract the row value: store the full call result to a stack
        // slot (address-taken local) and read the low byte back with lbz,
        // matching the retail shape (see CfGimmickSaveOff ctor pattern).
        u32 vfull = getBdatStringColumnValue(bdat, &str[0x38], idx);
        u8 v = *(u8*)&vfull;
        if (func_8019FB54(idx, &str[0x6], &str[0xe], &str[0x17],
                          &str[0x20], &str[0x26], &str[0x2c], &str[0x32]) != 0) {
            s32 word = idx >> 5;
            u32 w = bitmap[word];
            u32 bit = 1u << (idx & 31);
            if (w & bit) {
                bitmap[word] = w & ~bit;
                func_80462D04__8CTaskLODFv(v);
                func_804BCC30(func_804BC9EC__Fv(), v);
            }
        } else {
            s32 word = idx >> 5;
            u32 w = bitmap[word];
            u32 bit = 1u << (idx & 31);
            if (!(w & bit)) {
                bitmap[word] = w | bit;
                func_80462D5C__8CTaskLODFv(v);
                func_804BCC3C(func_804BC9EC__Fv(), v);
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Non-target functions kept from the scaffold so the unit still builds.
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// Target: func_8019F93C - approach-an Npc toward its movement target. Computes
// the target delta, drives the move helpers, and returns to idle (empty state
// fn) once the target is nearly reached or approach no longer applies.
// ---------------------------------------------------------------------------
namespace cf {
void func_8019F93C(CCtrlMoveNpc* self) {
    const ml::CVec3* pos =
        ((CNpcBaseData*)self->mBaseData)->field_0x28->getPos();

    // PS vector subtraction: delta = target - current position.
    ml::CVec3 diff = self->mField58 - *pos;
    ml::CVec3 v = diff;

    // Horizontal (XZ) distance squared to the target.
    const f32 len2 = v.x * v.x + v.z * v.z;

    if (len2 > lbl_eu_80667C6C) {
        ml::CVec3 v2;
        f32 speed = self->mField70;
        if (len2 < lbl_eu_80667C70) {
            if (len2 < lbl_eu_80667C5C) {
                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            // Norm the horizontal distance, guarding the epsilon case.
            f32 len;
            if (len2 > lbl_eu_80667C5C) {
                len = len2 * FrSqrt__Q24nw4r4mathFf(len2);
            } else {
                len = lbl_eu_80667C5C;
            }
            speed = (self->mField70 - lbl_eu_80667C58) * (len - lbl_eu_80667C60)
                    * lbl_eu_80667C74 + lbl_eu_80667C58;
        }

        v2.set(lbl_eu_80667C5C, lbl_eu_80667C5C, lbl_eu_80667C5C);
        func_80088974(self, &v2, &self->mField58, 1, 0);
        func_80089694(self, &v2, speed);
        if (func_800A5038(pos, &self->mField58, speed, lbl_eu_80667C60) == 0) {
            return;
        }
    }

idle:
    ((CNpcBaseData*)self->mBaseData)->field_0x14 = lbl_eu_80667C5C;
    if (self->mField6C < lbl_eu_80667C78) {
        func_80093618((CNpcBaseData*)self->mBaseData,
                      self->mField6C * lbl_eu_8066A210);
        // Retail duplicates this tail into both arms of the if/else.
        self->mStateFunc = __ptmf_null;
        func_8008962C(self);
    } else {
        self->mStateFunc = __ptmf_null;
        func_8008962C(self);
    }
}
}

// func_8019FB40 - clear the 16-byte Npc row bitmap.
void* func_8019FB40() {
    return memset(lbl_eu_805757E0, 0, 0x10);
}