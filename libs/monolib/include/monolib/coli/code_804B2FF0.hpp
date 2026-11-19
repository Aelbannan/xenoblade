#pragma once

#include <types.h>
#include <revolution/MTX.h>

// Partial layouts / forward decls for cross-TU data declared below (retail
// C-linkage pseudo-imports relocated from code_804B2FF0.cpp). Keep
// signatures/linkage identical - the names are the retail symbol names.
struct CColiBounds;
struct CColiHx;
struct CColiQx;
struct CColiWalkState;
struct CColiQueryNode;
struct CColiMoveNode;
struct CColiProcLocal;

// Collision-manager object behind sEneColiProbe.  The fields the shared
// collision helpers touch are the height reference at +0x60..+0x68 and the
// MEM1 allocation handle at +0x7C.
struct ColiProbeObj {
    u8 field_0x00[0x60]; // 0x00
    f32 field_0x60;      // 0x60
    f32 field_0x64;      // 0x64
    f32 field_0x68;      // 0x68
    u8 field_0x6C[0x7C - 0x6C]; // 0x6C
    u32 field_0x7C;      // 0x7C
};

// C-linkage imports
extern "C" CColiBounds* g_coliBounds;
extern "C" CColiHx* g_coliHalfExtent;
extern "C" CColiQx* g_coliXformPos;
extern "C" s16 g_coliNodeIndex; // master node index (lha)
extern "C" f32 lbl_eu_8066AEB0; // 0.0f
extern "C" f32 lbl_eu_8066AEB4; // 1.0f
extern "C" void ColiSetMoveEnableFlag(void*, int);
extern "C" CColiWalkState sColiWalkState;
extern "C" u8 sColiTargetTable;
extern "C" void __ct__9CColiProcFv(CColiProcLocal* self);
extern "C" f32 lbl_eu_8066AEC0;
extern "C" f32 lbl_eu_8066AEC4;
extern "C" f32 lbl_eu_8066AEC8;
extern "C" void ColiInitSegment(void*, const Vec*, const Vec*, void*);
extern "C" void ColiMakeAabbMinMax(void*, const Vec*, const Vec*);
extern "C" int ColiTestAabbContains(const void*, const CColiQueryNode*);
extern "C" int ColiProcessSegmentSelect(const CColiQueryNode*, const void*, int);
extern "C" void ColiNodeRefreshAxes(CColiMoveNode* node);
extern "C" void ColiNodeResortKeyed(CColiMoveNode* node);
extern "C" ColiProbeObj* sEneColiProbe;
