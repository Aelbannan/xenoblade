#pragma once

#include <types.h>
#include <nw4r/math.h>
#include <revolution/MTX.h>

// CREvtLight - realtime event light
// Inherits from cf::CREvtObj (vtable at 0x80532320, size 0x14)
// Own vtable at 0x80533D90 (size 0x28 = 10 entries)
// Total size: 0x50
struct CREvtLight {
    // Inherited from cf::CREvtObj (size 0x14)
    /* 0x00 */ void* vtable;
    /* 0x04 */ u32 field_04;
    /* 0x08 */ u32 ptmf[3];

    // CREvtLight fields
    /* 0x14 */ u32 field_14;  // initialized to 0
    /* 0x18 */ u32 field_18;  // constructor arg
    /* 0x1C */ u32 field_1C;  // set by func_801C36C4
    /* 0x20 */ u32 field_20;  // resource handle (0 initially)

    // Position/orientation of the light (used by func_801C37CC / func_801C3850)
    /* 0x24 */ u8 pad_24[0x3C - 0x24];
    /* 0x3C */ nw4r::math::VEC3 mPos;
    /* 0x48 */ u8 pad_48[0x4C - 0x48];
    /* 0x4C */ f32 mAngle;

    nw4r::math::VEC3* pos() { return &mPos; }
};

// Virtual-table interface for a position-getting target object.
// The declared virtual at +0xAC (slot 43) lines up with the retail vtable so
// MWCC emits the same r12 virtual-call sequence for the getter.
struct CREvtLightTargetIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v074();
    virtual void _v078();
    virtual void _v07C();
    virtual void _v080();
    virtual void _v084();
    virtual void _v088();
    virtual void _v08C();
    virtual void _v090();
    virtual void _v094();
    virtual void _v098();
    virtual void _v09C();
    virtual void _v0A0();
    virtual void _v0A4();
    virtual void _v0A8();
    virtual nw4r::math::VEC3* _v0AC();
};

// Small interface for the object pointed to by the game-manager sub-object at
// +0x2F3C. Its vtable slot 0x68 (index 26) is the "refresh/set-active" call.
struct CREvtLightNotifyIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068(int);
};