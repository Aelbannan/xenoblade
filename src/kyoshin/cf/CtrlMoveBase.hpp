#pragma once

#include <types.h>
#include <revolution/MTX.h>
#include "monolib/math/CVec3.hpp"

namespace cf {

    class CfObject; // real owner of position slot at +0xAC (CfObject_UnkVirtualFunc23)

    // CCtrlMoveBase layout: 0x48 bytes of data (field_0x00..field_0x46) then
    // the vptr for its single virtual unk08 at +0x48. Retail secondary table
    // lbl_eu_80527808 is the vtable for this class (RTTI,0,func_80089628).
    // Using __declspec(novtable) and manual vptr store keeps the vptr at +0x48
    // and makes this->unk08() dispatch via lwz 72(r31); lwz 8(r12) with
    // primary this (or r3,r31) exactly like retail.
    struct CCtrlMoveBaseFields {
        ml::CVec3 field_0x00;    // 0x00
        ml::CVec3 mPosition;     // 0x0C
        ml::CVec3 mVelocity;     // 0x18
        ml::CVec3 field_0x24;    // 0x24
        void* mpSomePtr;         // 0x30
        void* mpDataPtr;         // 0x34
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
    };

    struct CCtrlMoveData {
        u8 field_0x00[0xC];                // 0x00..0x0B
        f32 field_0x0C;                    // facing angle (rad)
        u8 field_0x10[4];                  // 0x10..0x13
        f32 field_0x14;                    // cached x component
        u8 field_0x18[0x10];               // 0x18..0x27
        CfObject* mPosObj;          // 0x28 real type is CfObject (vtable +0xAC is CfObject_UnkVirtualFunc23)
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
extern "C" int func_8047D2AC__17UnkClass_8047D2ACFv(void* a, void* b, void* c,
                                                     f32 d, f32 e, const void* f);
extern "C" int walkPathCheck__17UnkClass_8047D2ACFv(void* a, void* b, const void* c,
                                                     f32 d, f32 e, int f);
extern "C" int func_804BE398(void* vec, int a, int b, int c, f32 d, f32 e);
extern "C" void func_804BE4B4(void* out, int a);
extern "C" void func_804BE4E0(void* out, int a);

extern "C" void* lbl_eu_80527808[];
extern "C" int func_8047DC8C__17UnkClass_8047D2ACFv(void* a, void* b, void* c,
                                                     f32 d, f32 e, f32 f, int g);
extern "C" int func_8047DE3C__17UnkClass_8047D2ACFv(void* a, void* b,
                                                     f32 c, f32 d);
extern "C" int dispatchScnCommand__17UnkClass_8047D2ACFv(void* a, void* b,
                                                     f32 c, f32 d);

// Float constants in .sdata2 (values recovered from the retail binary).
extern const f32 lbl_eu_80666598;
extern const f32 lbl_eu_8066659C;
extern f32 lbl_eu_806665A0;   // 0.0f
extern f32 lbl_eu_806665A4;   // 1.0f
extern f32 lbl_eu_806665A8;
extern f32 lbl_eu_806665AC;
extern f32 lbl_eu_806665B0;
extern f32 lbl_eu_806665B4;
extern f32 lbl_eu_806665B8;
extern f32 lbl_eu_806665BC;
extern const f32 lbl_eu_8066A210;   // degrees-to-radians (declared const in other TUs)
extern f32 lbl_eu_8066AF20;
