// Decompiled unit kyoshin/cf/CtrlMoveNpc.
//
// CCtrlMoveNpc - NPC movement controller. Derived from CCtrlMoveBase (base
// occupies 0x00..0x4B; secondary vtable at 0x48 overwritten by the derived
// ctor). Own fields begin at 0x4C; mStateFunc (0x4C) is a ptmf, null by
// default and set to a real state function by beginNpcMove.
//
// Layout / types are defined here (this TU has no unit header); struct layouts
// recovered from the retail assembly in the session brief.

#include "kyoshin/harness_catalog.hpp"
#include <monolib/math/CVec3.hpp>
#include <revolution/MTX.h>
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (ml::epsilon)
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/CtrlMoveBase.hpp"
namespace nw4r { namespace math { f32 FrSqrt(f32 x); } }
#include "monolib/scn/code_804BC9EC.hpp"

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

// mBaseData @0x34 is CCtrlMoveData (from CtrlMoveBase.hpp): field_0x14 and
// mPosObj @0x28 (CfObject*). Typed here so callers skip void* casts.

class CCtrlMoveNpc {
public:
    CCtrlMoveNpc();

    // --- base class (CCtrlMoveBase) region 0x00..0x4B ---
    char mBase00[0x18];     // 0x00 primary fields
    ml::CVec3 mVec18;       // 0x18 mVelocity (CCtrlMoveBase::mVelocity)
    char mBase24[0x0C];     // 0x24..0x2F field_0x24
    void* mBase30;          // 0x30 mpSomePtr
    CCtrlMoveData* mBaseData; // 0x34 mpDataPtr
    f32 mBase38;            // 0x38 mFloatParam1
    f32 mBase3C;            // 0x3C mFloatParam2
    u16 mBase40;            // 0x40 mFlagsU16_1
    u16 mBase42;            // 0x42 mFlagsU16_2
    u8  mBase44;            // 0x44 mByte1
    u8  mBase45;            // 0x45 mByte2
    char mBase46[2];        // 0x46..0x47
    const char* mBaseVtable2; // 0x48 secondary vptr (CCtrlMoveBase::unk08's vtable, overwritten here)

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

// ---------------------------------------------------------------------------
// Imports
// ---------------------------------------------------------------------------

// Functions whose retail symbols are pre-mangled/unmangled names: declared with
// C linkage so the literal symbol name is resolved/emitted verbatim.
extern "C" {

// Base class ctor + ptmf runtime (CCtrlMoveBase's ctor, takes CCtrlMoveBase*).
void __ct__80088904(cf::CCtrlMoveBase* ths);
long __ptmf_test(PTMF* ptmf);
void __ptmf_scall(...);

// initial state-function ptmf triple (retail .data 0x80532DA8, 12 bytes).
extern u32 lbl_eu_80532DA8[3];

// bdat helpers / task helpers / movement helper (unmangled global symbols).
// Bdat_GetTable_AA34 and getBdatStringColumnValue come from kyoshin/code_801862C0.hpp.
// isNpcRowUsable is defined below; this prototype fixes its C linkage so the
// retail unmangled symbol name is emitted.
u32 Bdat_GetRowBase_B41C(void* bdat);
u32 Bdat_GetMaxRow_B1EC(void* bdat);
void activateLOD__8CTaskLODFv(s8 v);
void deactivateLOD__8CTaskLODFv(u8 v);
void ScnData_FwdB7D9C(void*, s8 v);
void ScnData_FwdB7DD4(void*, u8 v);
void maskMoveChildFlags(cf::CCtrlMoveBase* ths);
int isNpcRowUsable(u32 idx, const char* p1, const char* p2, const char* p3,
                  const char* p4, const char* p5, const char* p6, const char* p7);

// Movement base helpers / npc move helpers (unmangled global symbols).
void moveBaseUpdate(CCtrlMoveNpc* ths, const ml::CVec3* a, const ml::CVec3* b,
                   int c, int d);
void setMoveFacing(CCtrlMoveNpc* ths, const ml::CVec3* a, f32 f);
void moveBaseUnlink(CCtrlMoveNpc* ths);
void CtrlMoveEne_CommitHeadAngle(cf::CCtrlMoveData* data, f32 f);
int  VecMath_CircleOverlapXZ(const ml::CVec3* sub, const ml::CVec3* v, f32 f1, f32 f2);
void Warning__Q24nw4r2dbFPCciPCce(const char* file, int line, const char* fmt, ...);

// cf::CfGameManager::getResourceFromTable is declared non-static void in
// CfGameManager.hpp, but the retail call site passes no instance and uses the
// return value, so the header's declaration is wrong here. Declare the mangled
// symbol directly (canonical signature from kyoshin/code_801862C0.hpp).
u32 getResourceFromTable__Q22cf13CfGameManagerFv(u32 resourceId);

} // C-linkage import block

// Linker-label data imports. MWCC emits global-variable names unmangled.

// CCtrlMoveNpc secondary vtable (address-taken word table, so an array
// of pointers; the retail words are an sdata locator, zero, and a hook).
extern void* lbl_eu_80532DB4[];

// Warning() file / format rodata strings.
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

// ptr member-function null constant (3 words).
extern int (CCtrlMoveNpc::*const __ptmf_null)();

// sdata2 float literals referenced by beginNpcMove / tickNpcMove.
extern const f32 lbl_eu_80667C58;
extern const f32 lbl_eu_80667C5C;
extern const f32 lbl_eu_80667C60;
extern const f32 lbl_eu_80667C64;
extern const f32 lbl_eu_80667C68;
extern const f32 lbl_eu_80667C6C;
extern const f32 lbl_eu_80667C70;
extern const f32 lbl_eu_80667C74;
extern const f32 lbl_eu_80667C78[2];

// bdat column string table + row bitmap + other globals.
extern const char lbl_eu_80503D30[];
extern u32 lbl_eu_805757E0[];
extern void* lbl_eu_806640B4;   // active table bdat data
extern u32 lbl_eu_80663D90;     // global value copied into mField78

// ---------------------------------------------------------------------------
// __ct__cf_CtrlMoveNpc - constructor
// ---------------------------------------------------------------------------
namespace cf {
CCtrlMoveNpc::CCtrlMoveNpc() {
    __ct__80088904((cf::CCtrlMoveBase*)this);
    this->mBaseVtable2 = (const char*)lbl_eu_80532DB4;
    this->mStateFunc = __ptmf_null;
}
}

// ---------------------------------------------------------------------------
// NpcMove_DispatchState - dispatch the state ptmf if set; return "was set".
// ---------------------------------------------------------------------------
namespace cf {
extern "C" int NpcMove_DispatchState(CCtrlMoveNpc* ths) {
    if (ths->mStateFunc) {
        (ths->*ths->mStateFunc)();
    }
    return ths->mStateFunc != 0 ? 1 : 0;
}
}

// ---------------------------------------------------------------------------
// beginNpcMove - pick a movement target, initialise move fields and
// set the state fn. Approached-fraction is clamped into [0, max].
// ---------------------------------------------------------------------------
namespace cf {
extern "C" void beginNpcMove(CCtrlMoveNpc* ths, const ml::CVec3* vec, f32 scale, f32 paramB) {
    ths->mField64 = 4;
    ths->mField58 = *vec;
    ths->mField68 = scale;
    ths->mField6C = paramB;

    // The base data pointer is re-loaded for each access in the retail.
    ths->mField74 =
        *reinterpret_cast<float*>(ths->mBaseData->mPosObj->CfObject_getMoveRateScale());
    ths->mField78 = lbl_eu_80663D90;
    ths->mField70 = lbl_eu_80667C58;
    ths->mBaseData->field_0x14 = lbl_eu_80667C5C;

    // PS vector subtraction: delta = target - current position.
    ml::CVec3* pos = ths->mBaseData->mPosObj->CfObject_getPosVector();
    ml::CVec3 diff = *vec - *pos;
    float len2 = diff.x * diff.x + diff.z * diff.z;

    if (len2 < lbl_eu_80667C5C) {
        Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
    }
    // Normalise the horizontal distance, guarding the epsilon case.
    float sq;
    if (len2 > lbl_eu_80667C5C) {
        sq = len2 * nw4r::math::FrSqrt(len2);
    } else {
        sq = lbl_eu_80667C5C;
    }

    float t = lbl_eu_80667C64 * sq / scale + lbl_eu_80667C60;
    if (t > ml::epsilon) {
        float v = t / ths->mField74;
        ths->mField70 = v;
        if (v < lbl_eu_80667C58) {
            v = lbl_eu_80667C58;
        } else if (v > lbl_eu_80667C68) {
            v = lbl_eu_80667C68;
        }
        ths->mField70 = v;
    }

    maskMoveChildFlags((cf::CCtrlMoveBase*)ths);
    ths->mVec18 = ml::CVec3::zero;
    ths->mStateFunc = *reinterpret_cast<int (CCtrlMoveNpc::*const*)()>(lbl_eu_80532DA8);
}
}

// ---------------------------------------------------------------------------
// isNpcRowUsable - range check an Npc table row against row/value
// columns. Returns whether the row is "usable" for the current situation.
// C linkage inherited from the prototype above (retail symbol is unmangled).
// ---------------------------------------------------------------------------
int isNpcRowUsable(u32 idx, const char* p1, const char* p2, const char* p3,
                  const char* p4, const char* p5, const char* p6, const char* p7) {
    void* bdat = lbl_eu_806640B4;
    // Column values live in address-taken slots (full-word stores after each
    // call); the u8/u16 casts at each use site lower to lbz/lhz (retail
    // shape, cf. CfGimmickSaveOff ctor).
    u32 v0 = getBdatStringColumnValue(bdat, lbl_eu_80503D30, idx);
    u32 f = lbl_eu_80663E24;
    if ((f & (0x02000000u | 0x400u)) && *(u8*)&v0 != 0) {
        return 0;
    }

    u16 range0 = (u16)cf::CfGameManager::getQueuedFileEventCount();
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
        u32 range1 = getResourceFromTable__Q22cf13CfGameManagerFv((u32)*(u16*)&v10);
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
// refreshNpcRowFlags - walk the table rows, toggling a per-row bit in the
// bitmap and notifying LOD when the usable-flag flips.
// C linkage inherited from the CfGameManager.hpp declaration
// (retail symbol is the unmangled pre-mangled name).
// ---------------------------------------------------------------------------
extern "C" void refreshNpcRowFlags() {
    Bdat_GetTable_AA34();
    void* bdat = lbl_eu_806640B4;
    cf::CfGameManager::getGameSubManager();

    u32* bitmap;
    const char* str;
    s32 base = (s32)Bdat_GetRowBase_B41C(bdat);
    s32 count = (s32)Bdat_GetMaxRow_B1EC(bdat);
    bitmap = lbl_eu_805757E0;
    str = lbl_eu_80503D30;
    s32 end = base + count;

    for (s32 idx = base; idx < end; idx++) {
        // Byte-extract the row value: store the full call result to a stack
        // slot (address-taken local) and read the low byte back with lbz,
        // matching the retail shape (see CfGimmickSaveOff ctor pattern).
        u32 vfull = getBdatStringColumnValue(bdat, &str[0x38], idx);
        u8 v = *(u8*)&vfull;
        if (isNpcRowUsable(idx, &str[0x6], &str[0xe], &str[0x17],
                          &str[0x20], &str[0x26], &str[0x2c], &str[0x32]) != 0) {
            s32 word = idx >> 5;
            u32 w = bitmap[word];
            u32 bit = 1u << (idx & 31);
            if (w & bit) {
                bitmap[word] = w & ~bit;
                activateLOD__8CTaskLODFv(v);
                ScnData_FwdB7D9C(getScnHandle(), v);
            }
        } else {
            s32 word = idx >> 5;
            u32 w = bitmap[word];
            u32 bit = 1u << (idx & 31);
            if (!(w & bit)) {
                bitmap[word] = w | bit;
                deactivateLOD__8CTaskLODFv(v);
                ScnData_FwdB7DD4(getScnHandle(), v);
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Non-target functions kept from the scaffold so the unit still builds.
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// tickNpcMove - approach-an Npc toward its movement target. Computes
// the target delta, drives the move helpers, and returns to idle (empty state
// fn) once the target is nearly reached or approach no longer applies.
// ---------------------------------------------------------------------------
namespace cf {
void tickNpcMove(CCtrlMoveNpc* ths) {
    const ml::CVec3* pos =
        ths->mBaseData->mPosObj->CfObject_getPosVector();

    // PS vector subtraction: delta = target - current position.
    ml::CVec3 diff = ths->mField58 - *pos;
    ml::CVec3 v = diff;

    // Horizontal (XZ) distance squared to the target.
    const f32 len2 = v.x * v.x + v.z * v.z;

    if (len2 > lbl_eu_80667C6C) {
        ml::CVec3 v2;
        f32 speed = ths->mField70;
        if (len2 < lbl_eu_80667C70) {
            if (len2 < lbl_eu_80667C5C) {
                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            // Norm the horizontal distance, guarding the epsilon case.
            f32 len;
            if (len2 > lbl_eu_80667C5C) {
                len = len2 * nw4r::math::FrSqrt(len2);
            } else {
                len = lbl_eu_80667C5C;
            }
            speed = (ths->mField70 - lbl_eu_80667C58) * (len - lbl_eu_80667C60)
                    * lbl_eu_80667C74 + lbl_eu_80667C58;
        }

        v2.set(lbl_eu_80667C5C, lbl_eu_80667C5C, lbl_eu_80667C5C);
        moveBaseUpdate(ths, &v2, &ths->mField58, 1, 0);
        setMoveFacing(ths, &v2, speed);
        if (VecMath_CircleOverlapXZ(pos, &ths->mField58, speed, lbl_eu_80667C60) == 0) {
            return;
        }
    }

    // Idle tail: clear the approach rate; when paramB allows it, also run the
    // turn-rate helper before dropping back to the idle state function.
    ths->mBaseData->field_0x14 = lbl_eu_80667C5C;
    if (ths->mField6C < lbl_eu_80667C78[0]) {
        CtrlMoveEne_CommitHeadAngle(ths->mBaseData,
                      ths->mField6C * ml::deg2rad);
        ths->mStateFunc = __ptmf_null;
        moveBaseUnlink(ths);
    } else {
        ths->mStateFunc = __ptmf_null;
        moveBaseUnlink(ths);
    }
}
}

// NpcMove_ClearRowBmp - clear the 16-byte Npc row bitmap.
extern "C" void NpcMove_ClearRowBmp() {
    memset(lbl_eu_805757E0, 0, 0x10);
}
// ============================================================================
// Retail data owned by this TU. Defined here at EOF so -ipa codegen for
// the functions above is unaffected.
// ============================================================================

// Cross-TU data slots (flat retail names).
extern "C" void emptyMoveBaseHook();
extern u32 lbl_eu_80661C18;
extern u32 lbl_eu_80662548[];  // sdata word pair (defined below).

// [.data] 0x80532DA8-0x80532DB4 (12B): ptmf-shaped triple {0, -1, tickNpcMove}.
// Named with the retail label: the word-typed declaration above round-trips
// the 12-byte copy into mStateFunc with identical codegen.
u32 lbl_eu_80532DA8[3] = {
    0x00000000, 0xFFFFFFFF, (u32)&cf::tickNpcMove,
};

// [.data] 0x80532DB4-0x80532DC0 (12B): secondary vtable (sdata locator,
// zero, base hook); assigned to mBaseVtable2 by the ctor.
void* lbl_eu_80532DB4[3] = {
    (void*)&lbl_eu_80662548, (void*)0, (void*)&emptyMoveBaseHook,
};

// [.data] 0x80532DC0-0x80532DD0 (16B): sdata-referenced word + zero fill.
u32 lbl_eu_80532DC0[4] = {
    (u32)&lbl_eu_80661C18, 0x00000000, 0x00000000, 0x00000000,
};

// [.bss] 0x805757E0-0x80575918 (312B): movement state storage, five objects
// in retail order (all starts 16-aligned, so placement is exact).
u32 lbl_eu_805757E0[4];
f32 lbl_eu_805757F0[12];
f32 lbl_eu_80575820[14];
u32 lbl_eu_80575858[6];
u32 lbl_eu_80575870[42];

// [.rodata] 0x80503D20-0x80503D70 (80B): class-name string (unreferenced pool
// bytes, kept under the retail label) + the bdat column-name table.
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_80503D20[0x10] = {
    0x63,0x66,0x3A,0x3A,0x43,0x74,0x72,0x6C,0x4D,0x6F,0x76,0x65,0x4E,0x70,0x63,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
const char lbl_eu_80503D30[0x40] = {
    0x65,0x72,0x61,0x73,0x65,0x00,0x71,0x75,0x65,0x73,0x74,0x49,0x44,0x00,0x71,0x73,
    0x74,0x5F,0x73,0x66,0x6C,0x67,0x00,0x71,0x73,0x74,0x5F,0x65,0x66,0x6C,0x67,0x00,
    0x73,0x66,0x6C,0x67,0x31,0x00,0x65,0x66,0x6C,0x67,0x31,0x00,0x73,0x66,0x6C,0x67,
    0x32,0x00,0x65,0x66,0x6C,0x67,0x32,0x00,0x6D,0x6F,0x64,0x65,0x6C,0x00,0x00,0x00,
};

// [.sdata] 0x80662548-0x80662550 (8B): rodata-name + data-tail locator pair.
#pragma section data_type ".sdata" ".sdata"
extern __declspec(section ".sdata") u32 lbl_eu_80662548[2] = { (u32)&lbl_eu_80503D20, (u32)&lbl_eu_80532DC0 };
#pragma section data_type ".data" ".data"

// [.sdata2] 0x80667C58-0x80667C80 (40B): movement float table in retail order
// (splits .sdata2 80667C58-80): 0.1/0.0/0.2/3.6/1.0/~0.04/4.0/0.5/360.0+0.0.
// The ~0.04 slot uses the 0.040000003f spelling (same as CtrlNpc's table:
// plain 0.04f rounds to ...70A, retail is ...70B). lbl_eu_80667C78 is an
// 8-byte object (symbols.txt size 0x8), defined as an f32 pair; the code
// reads element [0]. Defined at EOF so the const values cannot propagate
// into the functions above (their lfs loads stay symbolic, as retail).
extern __declspec(section ".sdata2") const f32 lbl_eu_80667C58 = 0.1f;
extern __declspec(section ".sdata2") const f32 lbl_eu_80667C5C = 0.0f;
extern __declspec(section ".sdata2") const f32 lbl_eu_80667C60 = 0.2f;
extern __declspec(section ".sdata2") const f32 lbl_eu_80667C64 = 3.6f;
extern __declspec(section ".sdata2") const f32 lbl_eu_80667C68 = 1.0f;
extern __declspec(section ".sdata2") const f32 lbl_eu_80667C6C = 0.040000003f;
extern __declspec(section ".sdata2") const f32 lbl_eu_80667C70 = 4.0f;
extern __declspec(section ".sdata2") const f32 lbl_eu_80667C74 = 0.5f;
extern __declspec(section ".sdata2") const f32 lbl_eu_80667C78[2] = { 360.0f, 0.0f };
