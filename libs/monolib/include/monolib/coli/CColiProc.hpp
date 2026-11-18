#pragma once

#include <types.h>
#include <nw4r/math/math_types.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)

// CColiProc vtable: {RTTI__9CColiProc, offset-to-top 0, slot0 = ColiNoop} plus a
// trailing pad word; defined (and owned) by code_804A6C60.cpp. __ct__CColiProc
// installs it at this+0x0, so the class has exactly one virtual function.
extern "C" u32 __vt__9CColiProc[4];

// Authority: CColiProcTarget/CColiProc owned by CColiProc.cpp (defines TU).
// Single definition shared by CColiProc.cpp and code_804A6C60.cpp (which
// holds a 0x10-byte view and calls via ((CColiProc*)&proc)-> for correct
// retail relocs InitQuad__9CColiProcFP15CColiProcTargetUlUl etc.).
struct CColiProcTarget {
    u32 field_0x0;
    u16 field_0x4;
    u16 field_0x6;
};

struct CColiProc {
    CColiProcTarget* field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xC;
    u8 field_0x10[0x28];
    nw4r::math::VEC3 field_0x38;
    u32 field_0x44;
    void* field_0x48;
    u32 field_0x4C;
    void InitQuad(CColiProcTarget* target, u32 a, u32 b);
    void InitTriple(CColiProcTarget* target, u32 a);
    bool IsTargetFlagged();
    bool TestBit18State();
};
