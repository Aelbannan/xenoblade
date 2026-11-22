#pragma once

#include <types.h>
#include <revolution/MTX.h>
#include "monolib/math/CVec3.hpp"

namespace cf {

    class CfObject; // real owner of position slot at +0xAC (CfObject_getPosVector)
    struct CCtrlMoveData;

    // CCtrlMoveBase layout: 0x48 bytes of data (field_0x00..field_0x46) then
    // the vptr for its single virtual unk08 at +0x48. Retail secondary table
    // lbl_eu_80527808 is the vtable for this class (RTTI,0,emptyMoveBaseHook).
    // Using __declspec(novtable) and manual vptr store keeps the vptr at +0x48
    // and makes this->unk08() dispatch via lwz 72(r31); lwz 8(r12) with
    // primary this (or r3,r31) exactly like retail.
    struct CCtrlMoveBaseFields {
        ml::CVec3 field_0x00;    // 0x00
        ml::CVec3 mPosition;     // 0x0C
        ml::CVec3 mVelocity;     // 0x18
        ml::CVec3 field_0x24;    // 0x24
        void* mpSomePtr;         // 0x30
        CCtrlMoveData* mpDataPtr; // 0x34 owner object (ctor param)
        f32 mFloatParam1;        // 0x38
        f32 mFloatParam2;        // 0x3C
        u16 mFlagsU16_1;         // 0x40
        u16 mFlagsU16_2;         // 0x42
        u8 mByte1;               // 0x44
        u8 mByte2;               // 0x45
        u8 field_0x46[2];        // 0x46
    };

    class __declspec(novtable) CCtrlMoveBase : public CCtrlMoveBaseFields {
    public:
        CCtrlMoveBase(void* dataPtr);
        virtual void unk08(); // +0x08 in secondary vtable -> lbl_eu_80527808[2]

    protected:
        // novtable hierarchy: each ctor installs its retail vtable label in
        // the vptr slot (+0x48) by hand.
        void setVtable(const void* vtable) {
            *(const void**)((u8*)this + 0x48) = vtable;
        }
    };

    struct CCtrlMoveData {
        u8 field_0x00[0xC];                // 0x00..0x0B
        f32 field_0x0C;                    // facing angle (rad)
        u8 field_0x10[4];                  // 0x10..0x13
        f32 field_0x14;                    // cached x component
        u8 field_0x18[0x10];               // 0x18..0x27
        CfObject* mPosObj;          // 0x28 real type is CfObject (vtable +0xAC is CfObject_getPosVector)
    };

    // Interface dispatched through the secondary vtable pointer at
    // CCtrlMoveBase+0x48 (retail label lbl_eu_80527808). Only slot 0x08 is
    // ever called; modeled as raw function pointers so MWCC emits the direct
    // lwz/lwz/mtctr/bctrl sequence.
    // Sub-object at CfGameManager's instance slot +0x2F2C: its first word
    // decides whether the per-object move instance is created lazily.
    struct CCtrlMoveMgr2F2C {
        u32 field_0x00;
    };

} // namespace cf

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

extern "C" void* allocFreeNode__17UnkClass_8047CD0CFv();
extern "C" int runWalkerTick__17UnkClass_8047D2ACFv(void* a, void* b, void* c,
                                                     f32 d, f32 e, const void* f);
extern "C" int updateWalker__17UnkClass_8047D2ACFv(void* a, void* b, const void* c,
                                                     f32 d, f32 e, int f);
extern "C" int ScnRes_VertRayForward_E398(void* vec, int a, int b, int c, f32 d, f32 e);
extern "C" void ScnRes_CopyEntryHead(void* out, int a);
extern "C" void ScnRes_CopyEntryMid(void* out, int a);

extern "C" void* lbl_eu_80527808[];
extern "C" int findWalkRoute__17UnkClass_8047D2ACFv(void* a, void* b, void* c,
                                                     f32 d, f32 e, f32 f, int g);
extern "C" int solveWalkPair__17UnkClass_8047D2ACFv(void* a, void* b,
                                                     f32 c, f32 d);
extern "C" int dispatchScnCommand__17UnkClass_8047D2ACFv(void* a, void* b,
                                                     f32 c, f32 d);

// Float constants in .sdata2 (values recovered from the retail binary).
extern f32 lbl_eu_806665AC;
extern f32 lbl_eu_8066AF20;

// Canonical declarations for the free helpers defined in CtrlMoveBase.cpp.
// Forward declarations keep the caller TUs independent of the defining TU's
// private view types.
namespace ml { struct CVec3; }
struct maskMoveChildFlags_obj;

void moveBaseSyncVel(cf::CCtrlMoveBase* self);
void maskMoveBaseFlags(void* self);
void setMoveFacing(cf::CCtrlMoveBase* self, const ml::CVec3* vec, f32 speed);
void maskMoveChildFlags(maskMoveChildFlags_obj* self);
void writeMoveSlotFloat(void* obj, f32 value);
int moveBaseScnCmd(cf::CCtrlMoveBase* self);
int moveBaseQuery(cf::CCtrlMoveBase* self, ml::CVec3* out);
