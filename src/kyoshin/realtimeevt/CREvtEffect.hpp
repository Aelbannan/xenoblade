#pragma once

#include <types.h>
#include "kyoshin/realtimeevt/CREvtObj.hpp"

// CREvtEffect - realtime event effect controller
//
// Layout:
//   0x00-0x13: cf::CREvtObj base fields (vtable ptr + field_04 + __ptmf[3])
//   0x14:      secondary vtable pointer (callback interface for effect system)
//   0x18-0x143: effect-specific fields
//
// The secondary vtable at offset 0x14 is a callback interface used by the
// effect system. It has 3 virtual functions:
//   vtable[0]: dtor (thunk: func_80185688, adjusts this by -0x14)
//   vtable[1]: callback (thunk: func_80185680, adjusts this by -0x14)
//   vtable[2]: callback (direct: func_801855C4)
//
// Total size: 0x144

struct CREvtEffect {
    // cf::CREvtObj base at offset 0x00
    /* 0x00 */ void* vtable;
    /* 0x04 */ u32 field_04;
    /* 0x08 */ u32 ptmf[3];

    // Secondary vtable pointer (callback interface at offset 0x14)
    /* 0x14 */ void* mSecondaryVtable;

    // Effect-specific fields
    /* 0x18 */ void* mPtr18;
    /* 0x1C */ u32 mParam1C;
    /* 0x20 */ void* mModel;          // CLibLayoutModel*
    /* 0x24 */ void* mMaterial;
    /* 0x28 */ void* mAnim28;
    /* 0x2C */ void* mBdatEntry;
    /* 0x30 */ void* mEffects[32];    // effect pointer array
    /* 0xB0 */ s32 mEffectCount;
    /* 0xB4 */ void* mNodeB4;
    /* 0xB8 */ void* mNodeB8;
    /* 0xBC */ void* mNodeBC;
    /* 0xC0 */ s32 mNodeIdC0;
    /* 0xC4 */ s32 mNodeIdC4;
    /* 0xC8 */ s32 mNodeIdC8;
    /* 0xCC */ f32 mAnm1Translate[3];
    /* 0xD8 */ f32 mAnm1Rotate[3];
    /* 0xE4 */ f32 mAnm1Scale[3];
    /* 0xF0 */ f32 mAnm2Scale[3];
    /* 0xFC */ f32 mAnm2Rotate[3];
    /* 0x108 */ f32 mAnm2Translate[3];
    /* 0x114 */ f32 mAnm3Translate[3];
    /* 0x120 */ f32 mAnm3Scale[3];
    /* 0x12C */ f32 mFloat12C;
    /* 0x130 */ s32 mLastFrame;
    /* 0x134 */ u8 mFlag134;
    /* 0x138 */ void* mBdatData;
    /* 0x13C */ s32 mTime;
    /* 0x140 */ s32 mType;
};