#pragma once

#include <types.h>

// CREvtLight - realtime event light
// Inherits from cf::CREvtObj (vtable at 0x80532320, size 0x14)
// Own vtable at 0x80533D90 (size 0x28 = 10 entries)
// Total size: 0x24
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
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

// Own vtable (defined in this TU)
extern "C" void* lbl_eu_80533D90[];

// Math helpers (nw4r math / PPC PS vector math)
extern "C" f32 Atan2FIdx__Q24nw4r4mathFff(f32 y, f32 x);
extern "C" f32 PSVECMag(const f32* vec);