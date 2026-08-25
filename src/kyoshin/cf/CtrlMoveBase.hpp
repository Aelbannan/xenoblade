#pragma once

#include <types.h>
#include <revolution/MTX.h>
#include "monolib/math/CVec3.hpp"

namespace cf {

    class CCtrlMoveBase; // fwd: referenced by CCtrlMoveBaseIface below

    // Interface dispatched through the secondary vtable pointer at
    // CCtrlMoveBase+0x48 (retail label lbl_eu_80527808). Only slot 0x08 is
    // ever called; modeled as raw function pointers so MWCC emits the direct
    // lwz/lwz/mtctr/bctrl sequence.
    struct CCtrlMoveBaseIface {
        void (*vf00)();
        void (*vf04)();
        void (*unk08)(CCtrlMoveBase* self);
    };

    // Base class for move-control objects (CCtrlMovePC, CCtrlMoveEne, CCtrlMoveNpc, etc.)
    // Manages position, velocity, and movement state for controllable entities.
    //
    // Layout (from constructor at 0x800892DC):
    //   0x00: parent vtable / first field
    //   0x0C: mPosition (CVec3, 12 bytes)
    //   0x18: mVelocity (CVec3, 12 bytes)
    //   0x24: padding/unknown (12 bytes)
    //   0x30: mpSomePtr (initialized to NULL)
    //   0x34: mpDataPtr (set from constructor param)
    //   0x38: mFloatParam1 (from lbl_80666598)
    //   0x3C: mFloatParam2 (from lbl_8066659C)
    //   0x40: mFlagsU16_1
    //   0x42: mFlagsU16_2
    //   0x44: mByte1
    //   0x45: mByte2
    //   0x48: secondary vtable (interface or adjusted base)
    // Minimum size: 0x4C
    class CCtrlMoveBase {
    public:
        CCtrlMoveBase(void* dataPtr);

        // 0x04..0x0B unknown base fields
        ml::CVec3 field_0x00;    // 0x00 (not initialized by ctor)
        ml::CVec3 mPosition;     // 0x0C
        ml::CVec3 mVelocity;     // 0x18
        ml::CVec3 field_0x24;    // 0x24 (cached move direction)
        void* mpSomePtr;         // 0x30 (per-object instance, NULL-init)
        void* mpDataPtr;         // 0x34 (ctor param: move data block)
        f32 mFloatParam1;        // 0x38
        f32 mFloatParam2;        // 0x3C
        u16 mFlagsU16_1;         // 0x40
        u16 mFlagsU16_2;         // 0x42
        u8 mByte1;               // 0x44
        u8 mByte2;               // 0x45
        u8 field_0x46[2];        // 0x46
        void* mSecondaryVtable;  // 0x48
    };

    // Move data block (mpDataPtr). The object at +0x28 exposes its position
    // through vtable slot 0xAC (43rd virtual). Declared with 43 virtual
    // slots so MWCC's virtual-call codegen (lwz r12,0(r3); lwz r12,0xac(r12))
    // matches the retail; the class is never instantiated here.
    class CCtrlMovePosObj {
    public:
        virtual void vf00();
        virtual void vf01();
        virtual void vf02();
        virtual void vf03();
        virtual void vf04();
        virtual void vf05();
        virtual void vf06();
        virtual void vf07();
        virtual void vf08();
        virtual void vf09();
        virtual void vf10();
        virtual void vf11();
        virtual void vf12();
        virtual void vf13();
        virtual void vf14();
        virtual void vf15();
        virtual void vf16();
        virtual void vf17();
        virtual void vf18();
        virtual void vf19();
        virtual void vf20();
        virtual void vf21();
        virtual void vf22();
        virtual void vf23();
        virtual void vf24();
        virtual void vf25();
        virtual void vf26();
        virtual void vf27();
        virtual void vf28();
        virtual void vf29();
        virtual void vf30();
        virtual void vf31();
        virtual void vf32();
        virtual void vf33();
        virtual void vf34();
        virtual void vf35();
        virtual void vf36();
        virtual void vf37();
        virtual void vf38();
        virtual void vf39();
        virtual void vf40();
        virtual ml::CVec3* getPosition();
    };

    struct CCtrlMoveData {
        u8 field_0x00[0xC];                // 0x00..0x0B
        f32 field_0x0C;                    // facing angle (rad)
        u8 field_0x10[4];                  // 0x10..0x13
        f32 field_0x14;                    // cached x component
        u8 field_0x18[0x10];               // 0x18..0x27
        CCtrlMovePosObj* mPosObj;          // 0x28
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

extern "C" void* func_8047CE7C__17UnkClass_8047CD0CFv();
extern "C" int func_8047D2AC__17UnkClass_8047D2ACFv(void* a, void* b, void* c,
                                                     f32 d, f32 e, const void* f);
extern "C" int func_8047DD4C__17UnkClass_8047D2ACFv(void* a, void* b, const void* c,
                                                     f32 d, f32 e, int f);
extern "C" int func_804BE398(void* vec, int a, int b, int c, f32 d, f32 e);
extern "C" void func_804BE4B4(void* out, int a);
extern "C" void func_804BE4E0(void* out, int a);

extern "C" void* lbl_eu_80527808[];
extern "C" int func_8047DC8C__17UnkClass_8047D2ACFv(void* a, void* b, void* c,
                                                     f32 d, f32 e, f32 f, int g);
extern "C" int func_8047DE3C__17UnkClass_8047D2ACFv(void* a, void* b,
                                                     f32 c, f32 d);
extern "C" int func_8047DE14__17UnkClass_8047D2ACFv(void* a, void* b,
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
