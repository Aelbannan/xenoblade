#pragma once

#include <types.h>
#include "kyoshin/realtimeevt/CREvtObj.hpp"
#include "monolib/core/CSchedule.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)

// ---------------------------------------------------------------------------
// Opaque helper objects touched by this TU (only the fields used here are
// declared; the rest of each object is unknown).
// ---------------------------------------------------------------------------

// Effect instance created by EffRoot_AddSchedReq. The rotation/scale params at
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

// Minimal view of the layout model object created by scnImN4BuildByIdx
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

namespace cf {

// cf::CREvtEffect - realtime-event effect (kind 1, 0x144 bytes).
// Bases (retail vtable 0x805322D8 + RTTI base list 0x80532314): public
// cf::CREvtObj at 0x00, private CScheduleChild at 0x14 (it registers itself
// on the effects it spawns; ~CSchedule calls OnScheduleDelete). The direct
// OnScheduleDelete entry is appended after the CScheduleChild vtable.
class CREvtEffect : public CREvtObj, CScheduleChild {
public:
    CREvtEffect(CREvtParam* param);
    virtual ~CREvtEffect();
    virtual void vfunc_04();
    virtual void vfunc_08();
    virtual int vfunc_0C();
    virtual void vfunc_1C();
    virtual void OnScheduleDelete(CSchedule* pSchedule);

    void refreshChrAnim();
    bool spawnEffect();
    void updateEffects();
    void initModel(void* src, void* bdatEntry);

    // CScheduleChild subobject at 0x14
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

} // namespace cf

using cf::CREvtEffect;

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

// Bdat / data loading
extern "C" u32 EvtSeqResolveListEntryAddr(int idx);
extern "C" void* EvtSeqFindEventByNameData(void* ptr);
extern "C" u32 EvtSeqGetCounter100();
extern "C" u32 EvtSeqIsSlotIndexFour();
extern "C" unsigned char REvtCam_GetFieldA5Flag();
extern "C" int EvtSeqResolveAnmChr(u8* data, const char* name, s32* out);

// Effect system
extern "C" void EffSched_LookupA(void* mgr, void* data);
extern "C" void EffSched_LookupB(void* mgr, void* data);
extern "C" void* EffRoot_AddSchedReq(void* mgr, void* bdat, void* global, int r6, int r7, int r8);
extern "C" void schedClearFlag15Update(void* effect);
extern "C" void schedAttachChildSlot(void* effect, void* parent);
extern "C" void schedSetStepIntervals(void* effect, f32 f1, f32 f2);
extern "C" void schedDetachChildSlot(void* effect, void* parent);

// Model / animation
extern "C" nw4r::g3d::ChrAnmResult* scnImN4AnimFn(void* model, const char* animName, f32 time);
extern "C" u8* scnImN4BuildByIdx(u8* global, u8* handle, int, int, int, int);
extern "C" void simSetLeafDist7B0(void* self, f32 value);
extern "C" void simSetFlag2000Chain(void* model, int val);
extern "C" void simBindChrAnim(void* model, void* animData, int, int, int, int, int);

// Memory
extern "C" bool Scn_IsAnimActiveOrNull(void* ptr);
extern "C" void* Scn_InitGlobalA(void* global, void* bdat, int val);

// Random

// Device
// getLanguage is the real static member CDeviceSC::getLanguage; callers
// include monolib/device/CDeviceSC.hpp (no pre-mangled extern here).
extern "C" bool getStaticFileData__14CLibStaticDataFPCcP16StaticDataHandlePUl(
    const char* name, void* handle, u32* outSize);

// Globals
extern "C" {
    extern u32 lbl_eu_8065FC18[];    // effect-singleton manager object (array forces @ha/@l)
    extern const char lbl_eu_80503800[]; // "EFFMonochrome\0MonoRGB\0"
}
