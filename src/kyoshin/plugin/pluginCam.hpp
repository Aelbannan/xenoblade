#pragma once

#include "monolib/vm/yvm2.h"

// OC instance handle returned by vmArgOCGet: +0x04 is the object id
// consumed by the camera API (setTarget / setPosOfs / setLookatOfs / keyBegin).
struct CamOCHandle {
    u8 field_0x00[0x04];
    u32 field_0x04; // object id
};

// .sdata2 float-pool constants (retail symbol names).
extern const f32 lbl_eu_80666168; // 4096.0f fixed-point scale
// 0x4330000080000000: MWCC int->float conversion magic (2^52 + 0x8000).
extern const f64 lbl_eu_80666170;
extern const f32 lbl_eu_80666178; // 0.0f
extern const f32 lbl_eu_8066617C; // 1.0f
extern const f32 lbl_eu_8066A20C; // radians-to-degrees (57.2958...)
extern const f32 lbl_eu_8066A210; // degrees-to-radians (0.017453...)

// u32 -> float conversion: builds the 0x43300000-prefixed bit pattern in
// memory and subtracts the shared sdata2 magic double so the pool reloc
// matches the retail constant instead of an MWCC-synthesised @N entry
// (CfMapEffectManager.hpp convention).
union F64Conv {
    u32 w[2];
    f64 d;
};

// Camera state object reached via the camera interface's +0x0C pointer:
// position at +0x10C and rotation (radians) at +0x118, read by getPos/getRot.
struct CamStateObj {
    u8 field_0x00[0x10C - 0x00];
    f32 field_0x10C; // pos x
    f32 field_0x110; // pos y
    f32 field_0x114; // pos z
    f32 field_0x118; // rot x (radians)
    f32 field_0x11C; // rot y (radians)
    f32 field_0x120; // rot z (radians)
};

extern "C" int select(VMThread* pThread);
extern "C" int restore(VMThread* pThread);
extern "C" int setPos(VMThread* pThread);
extern "C" int setLookat(VMThread* pThread);
extern "C" int setDir(VMThread* pThread);
extern "C" int setRotX(VMThread* pThread);
extern "C" int setRotY(VMThread* pThread);
extern "C" int setFov(VMThread* pThread);
extern "C" int setTarget(VMThread* pThread);
extern "C" int setPosOfs(VMThread* pThread);
extern "C" int setLookatOfs(VMThread* pThread);
extern "C" int getPos(VMThread* pThread);
extern "C" int getRot(VMThread* pThread);
extern "C" int keyBegin(VMThread* pThread);
extern "C" int keyEnd(VMThread* pThread);
extern "C" int keyAdd(VMThread* pThread);
extern "C" int isKeyMove(VMThread* pThread);
extern "C" int waitKeyMove(VMThread* pThread);
extern "C" int shake(VMThread* pThread);
extern "C" int stopShake(VMThread* pThread);

void pluginCamRegist();