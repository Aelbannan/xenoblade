#pragma once

#include <types.h>
#include "kyoshin/realtimeevt/CREvtObj.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

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

// ---------------------------------------------------------------------------
// Opaque helper objects touched by this TU (only the fields used here are
// declared; the rest of each object is unknown).
// ---------------------------------------------------------------------------

// Effect instance created by func_804CC1F4. The rotation/scale params at
// 0x1C-0x4C are raw 32-bit word copies of the CREvtEffect anim fields
// (retail copies them with lwz/stw, so they are declared s32 here).
struct CEffectInst {
    /* 0x00 */ u16 mFlags;         // bit 0x4000 = parent-bound
    /* 0x02 */ u8 mPad2[0x12];
    /* 0x14 */ void* mParent;
    /* 0x18 */ u8 mPad18[0x4];
    /* 0x1C */ s32 mRot[3];        // rotation params (mAnm2Translate raw copy)
    /* 0x28 */ s32 mRotScaled[3];  // rotation * frame-rate divisor
    /* 0x34 */ s32 mScale[3];      // mAnm2Scale raw copy
    /* 0x40 */ s32 mScale2[3];     // mAnm3Scale raw copy
    /* 0x4C */ s32 mFloat4C;       // mFloat12C raw copy
    /* 0x50 */ f32 mPosX;
    /* 0x54 */ u8 mPad54[0x4];
    /* 0x58 */ u8 mPriority;
    /* 0x59 */ u8 mField59;
    /* 0x5A */ u8 mPad5A[0x2];
    /* 0x5C */ s32 mMode;
};

// Minimal view of the layout model object created by func_80489A60
// (only the fields touched by this TU are declared).
struct CLibLayoutModel {
    /* 0x00 */ void* vtable;
    /* 0x04 */ u8 mPad4[0x7A4];
    /* 0x7A8 */ u32 mFlags;
};

// Param block passed to the CREvtEffect constructor (r4).
struct CREvtParam {
    /* 0x00 */ u8 mPad0[0x0C];
    /* 0x0C */ u32 mLangInfo;   // language byte in bits 24..31
    /* 0x10 */ u32 mBdatId;
};

// Bdat animation entry (mBdatEntry).
struct CBdatEntry {
    /* 0x00 */ u8 mPad0[0x04];
    /* 0x04 */ s32 mPriority;
    /* 0x08 */ void* mNameData;
};

struct CREvtEffect {
    // cf::CREvtObj base at offset 0x00
    /* 0x00 */ void* vtable;
    /* 0x04 */ u32 field_04;
    /* 0x08 */ u32 ptmf[3];

    // Secondary vtable pointer (callback interface at offset 0x14)
    /* 0x14 */ void* mSecondaryVtable;

    // Effect-specific fields
    /* 0x18 */ void* mPtr18;
    /* 0x1C */ CREvtParam* mParam1C;
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

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - do not "fix" the mangled forms).
// Relocated from CREvtEffect.cpp so every TU that uses these symbols gets them
// from this header instead of inline pseudo-import declarations.
// ---------------------------------------------------------------------------

namespace nw4r {
namespace g3d {
struct ChrAnmResult;
}
} // namespace nw4r::g3d

// CREvtObj base-class functions
// (defined in kyoshin/realtimeevt/CREvtObj.cpp / code_80185754 TU)
extern "C" void __ct__cf_CREvtObj(cf::CREvtObj* self, int arg);
extern "C" void __dt__Q22cf8CREvtObjFv(cf::CREvtObj* self, int dealloc_flag);
extern "C" void __dt__80185754(void* ptr);

// Bdat / data loading
extern "C" void* func_8016A24C(u32 arg);
extern "C" void* func_8016A27C(void* ptr);
extern "C" int func_8016A35C();
extern "C" bool func_8016C410();
extern "C" bool func_80180954();
extern "C" void func_8016AF4C(void* obj, const char* name, s32* out);

// Effect system
extern "C" void func_804CC1BC(void* mgr, void* data);
extern "C" void func_804CC1D8(void* mgr, void* data);
extern "C" void* func_804CC1F4(void* mgr, void* bdat, void* global, int r6, int r7, int r8);
extern "C" void func_804E3CCC(void* effect);
extern "C" void func_804E3D0C(void* effect, void* parent);
extern "C" void func_804E3CDC(void* effect, f32 f1, f32 f2);
extern "C" void func_804E3D48(void* effect, void* parent);

// Model / animation
extern "C" nw4r::g3d::ChrAnmResult* func_8048BAD4(void* model, const char* animName, f32 time);
extern "C" void* func_80489A60(void* global, void* handle, int, int, int, int);
extern "C" void func_80484E5C(void* model, f32 val);
extern "C" void func_804827DC(void* model, int val);
extern "C" void func_804839D4(void* model, void* animData, int, int, int, int, int);

// Memory
extern "C" void func_80495E60(void* ptr);
extern "C" void* func_80495EAC(void* global, void* bdat, int val);

// Random

// Device
extern "C" int getLanguage__9CDeviceSCFv();
extern "C" bool getStaticFileData__14CLibStaticDataFPCcP16StaticDataHandlePUl(
    const char* name, void* handle, u32* outSize);

// Globals (retail linker symbols)
extern "C" {
    extern void* lbl_eu_805322D8[];
    extern u32 lbl_eu_8065FC18[];    // effect-singleton manager object (array forces @ha/@l)
    extern const char* lbl_eu_80662470;  // "effTgt"
    extern const char* lbl_eu_80662474;  // "effAtr"
    extern const char* lbl_eu_80662478;  // "eff"
    extern const char lbl_eu_80503800[]; // "EFFMonochrome\0MonoRGB\0"
    extern const f32 lbl_eu_8066793C;    // 1.0f
    extern const f32 lbl_eu_80667940;    // 0.0f
    extern const f32 lbl_eu_80667944;    // epsilon
    extern const f32 lbl_eu_80667948;    // 1.0f threshold
    extern const f32 lbl_eu_8066794C;    // 0.0f
    extern const double lbl_eu_80667950; // double magic for int->float
    extern const f32 lbl_eu_80667958;    // -1.0f
    extern const f32 lbl_eu_8066795C;    // frame rate divisor
    extern const f32 lbl_eu_80667960;    // 0.5f
    extern const f32 lbl_eu_8066A1F8;    // 60.0f
}

// Same-TU forward declarations (functions defined in CREvtEffect.cpp; extern
// "C" keeps callers emitting real unmangled bl branches to the retail symbols)
extern "C" void func_80184D90(CREvtEffect* self);
extern "C" bool func_80184F90(CREvtEffect* self);
extern "C" void func_8018515C(CREvtEffect* self);