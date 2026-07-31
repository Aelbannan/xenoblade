#pragma once

#include <types.h>
#include <nw4r/math.h>

namespace cf {

class CREvtObj;

} // namespace cf

// Forward declare for effect callback interface
struct CREvtEffectCallbackInterface {
    virtual ~CREvtEffectCallbackInterface();
    virtual void func_80185680(void* effect);
    virtual void func_801855C4(void* effect);
};

class CREvtEffect : public cf::CREvtObj, public CREvtEffectCallbackInterface {
public:
    CREvtEffect(u32 param);
    virtual ~CREvtEffect();

    // Virtual functions
    virtual void func_8018567C();
    virtual void func_80184D18();
    virtual u32 func_80185674();
    // Slot 4: inherited from base (func_80169048)
    // Slot 5: inherited from base (func_801809A8)
    // Slot 6: inherited from base (func_801696C4)
    virtual void func_80184D5C();

    // CREvtEffectCallbackInterface overrides
    virtual void func_80185680(void* effect);
    virtual void func_801855C4(void* effect);

    void func_80184D90();
    bool func_80184F90();
    void func_8018515C();
    void func_80185378(void* r4, void* r5);

    // CREvtObj fields: 0x00-0x13
    // 0x04: u32 mParam
    // 0x08: ptmf (3 * u32)

    // 0x14: secondary vtable pointer (CREvtEffectCallbackInterface)

    // Fields
    /* 0x18 */ void* mPtr18;
    /* 0x1C */ u32 mParam1C;
    /* 0x20 */ void* mModel;        // CLibLayoutModel*
    /* 0x24 */ void* mMaterial;     // some material/animation data
    /* 0x28 */ void* mAnim28;
    /* 0x2C */ void* mBdatEntry;
    /* 0x30 */ void* mEffects[32];  // effect pointer array
    /* 0xB0 */ s32 mEffectCount;
    /* 0xB4 */ void* mNodeB4;
    /* 0xB8 */ void* mNodeB8;
    /* 0xBC */ void* mNodeBC;
    /* 0xC0 */ s32 mNodeIdC0;
    /* 0xC4 */ s32 mNodeIdC4;
    /* 0xC8 */ s32 mNodeIdC8;
    /* 0xCC */ nw4r::math::VEC3 mAnm1Translate;
    /* 0xD8 */ nw4r::math::VEC3 mAnm1Rotate;
    /* 0xE4 */ nw4r::math::VEC3 mAnm1Scale;
    /* 0xF0 */ nw4r::math::VEC3 mAnm2Scale;
    /* 0xFC */ nw4r::math::VEC3 mAnm2Rotate;
    /* 0x108 */ nw4r::math::VEC3 mAnm2Translate;
    /* 0x114 */ nw4r::math::VEC3 mAnm3Translate;
    /* 0x120 */ nw4r::math::VEC3 mAnm3Scale;
    /* 0x12C */ f32 mFloat12C;
    /* 0x130 */ s32 mLastFrame;
    /* 0x134 */ u8 mFlag134;
    /* 0x138 */ void* mBdatData;
    /* 0x13C */ s32 mTime;
    /* 0x140 */ s32 mType;
};

extern "C" {
    // Adjustor thunks for the secondary vtable
    void func_80185680(void* self);
    void func_80185688(void* self);
    u32 func_80185674(void* self);
}