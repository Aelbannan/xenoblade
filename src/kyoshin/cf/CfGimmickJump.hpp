#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObject.hpp"

namespace cf {

class CfGimmickJump;

// Work object returned by createBattleActor (CfGameManager's per-gimmick
// manager object). Retail factory builds it as a CfObject-derived with
// vtable lbl_eu_80528600 (cf::CfObjectColl) or similar; the only slots
// used here are +0x9C setPosition (CfObject_UnkVirtualFunc19) and
// +0xC4 setHeight (CfObject_UnkVirtualFunc29, float). The back-pointer
// at +0xB0 is the CfObject::mSubObjB0 field (0xB0) reused as the
// owning gimmick link.
class __declspec(novtable) CfGimmickWork : public CfObject {
public:
    // CfObject is 0x70; gap to 0xB0 back-pointer
    u8 _gap70[0x40];
    CfGimmickJump* field_B0; // +0xB0 back-pointer to owning gimmick
};

struct CfGimmickJumpVec3 {
    f32 x;
    f32 y;
    f32 z;
};

struct CfGimmickJumpStateData {
    u32 words[0x10];
    f32 floatValue10; // word 0x10 - bulk-copied as float by retail
    u32 lastWord;     // word 0x11
};

/*
 * The jump gimmick is a flat object in the retail split.  Its common
 * gimmick state occupies the first 0x130 bytes; the fields below are the
 * jump-specific state read by the update callbacks.
 */
class __declspec(novtable) CfGimmickJump {
public:
    virtual ~CfGimmickJump();

    /* 0x00 */ CfGimmickJumpVec3 position;
    /* 0x10 */ CfGimmickJumpVec3 rotation;
    /* 0x1C */ CfGimmickJumpStateData initialState;
    /* 0x64 */ u16 bdatRow;
    /* 0x66 */ u16 flags66;
    /* 0x68 */ u16 field68;
    /* 0x6A */ u16 resourceId;
    /* 0x6C */ u16 field6C;
    /* 0x6E */ u16 field6E;
    /* 0x70 */ u8 lod[4];
    /* 0x74 */ u32 flags;
    /* 0x78 */ CfGimmickWork* linkedObject;
    /* 0x7C */ void* effect;
    /* 0x80 */ u16 soundHandle;
    /* 0x82 */ u16 gimmickType;
    /* 0x84 */ u16 duration;
    /* 0x86 */ u16 effectId;
    /* 0x88 */ u8 effectFlags;
    /* 0x89 */ u8 jumpFlags;
    /* 0x8A */ u16 waitFrames;
    /* 0x8C */ u16 jumpFrames;
    /* 0x90 */ f32 speed;
    /* 0x94 */ f32 height;
    /* 0x98 */ f32 frameScale;
    /* 0x9C */ f32 maxFrameScale;
    /* 0xA0 */ CfGimmickJumpStateData savedState;
    /* 0xE8 */ CfGimmickJumpVec3 transformedPosition;
    /* 0xF4 */ u8 gapF4[0x38];
    /* 0x12C */ u32 state;
    /* 0x130 */ f32 targetX;
    /* 0x134 */ f32 targetY;
    /* 0x138 */ f32 targetZ;
    /* 0x13C */ f32 targetAngle;
    /* 0x140 */ f32 timer;
    /* 0x144 */ f32 playerHeight[3];
    /* 0x150 */ f32 playerDeltaX[3];
    /* 0x15C */ f32 playerDeltaZ[3];
    /* 0x168 */ f32 verticalOffset;
    /* 0x16C */ u16 motionState;
    /* 0x16E */ u16 padding16E;
};

} // namespace cf
