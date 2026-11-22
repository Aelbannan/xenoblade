#pragma once

#include <types.h>
#include <revolution/GX.h>
#include <revolution/gx/GXVert.h>
#include "monolib/math/CMat34.hpp"

// Foreign .sdata/.sdata2 float constants referenced by this TU.
extern float lbl_eu_80663964;
extern const float lbl_eu_8066A948;
extern const float lbl_eu_8066A994;
extern const float lbl_eu_8066A9CC;

// Minimal nw4r g3d handle wrappers (single-pointer value types).
namespace nw4r {
namespace g3d {
struct ResMdl { void* ptr; };
struct ResNode { void* ptr; };
struct ResDic { void* ptr; };
} // namespace g3d
} // namespace nw4r

// Simple monolib vector used by the shadow draw path.
struct mlCVec3 {
    float x, y, z;
};

// Scene-level context shared by the maru-shadow functions.
// Large; only fields touched by this TU are named.
struct ShadowCtx {
    u8 _pad0[0x4];
    u32 field_0x04;
    u8 _pad1[0x2D0];
    u32 field_0x7a4; // config flags
    u8 _pad5[0x934];
    float field_0x2DC;
    float field_0x2E0;
    float field_0x2E4;
    float field_0x2E8;
    u8 _pad2[0x18];
    float field_0x304;
    u8 _pad3[0x1164];
    nw4r::g3d::ResMdl resMdl; // 0x146c
    void* field_0x147C;       // -> render-record array (elems + 0xec)
    u8 _pad4[0x2C];
    ml::CMat34* field_0x14AC;
};

class CScnMaruShadowNw4r;

// nw4r g3d / db imports used by this TU.
extern "C" {
extern int GetResNodeNumEntries__Q34nw4r3g3d6ResMdlCFv(nw4r::g3d::ResMdl mdl);
extern nw4r::g3d::ResNode GetResNode__Q34nw4r3g3d6ResMdlCFUl(nw4r::g3d::ResMdl mdl, unsigned long index);
extern nw4r::g3d::ResNode GetResNode__Q34nw4r3g3d6ResMdlCFPCc(nw4r::g3d::ResMdl mdl, const char* name);
extern void* GetResUserData__Q34nw4r3g3d7ResNodeFv(nw4r::g3d::ResNode node);
extern void* __vc__Q34nw4r3g3d6ResDicCFPCc(nw4r::g3d::ResDic dic, const char* key);
extern void Warning__Q24nw4r2dbFPCciPCce(const char* file, int line, const char* fmt, ...);

// monolib helpers called from the maru-shadow draw path.
extern void* Scn_GetCamItemCC(u32 unk, int id);
extern void* Scn_GetCamItem9C(u32 unk, int id);
extern void* getScnHandle();
extern int ScnData_FwdBC9A0();
extern bool ScnRes_VertRayForward_E398(mlCVec3* out, float x, float y, int a, int b);
extern bool ScnRes_CopyEntryHead(mlCVec3* v, int flag);
extern bool ScnRes_CopyEntryMid(mlCVec3* v, int flag);
extern void* ScnRes_GetEntryPtr_E50C(int idx);
// Canonical void*(void*) act-data accessor from CScnItemModel.cpp.
extern "C" void* simGetLeafActData(void* object);
extern void MaruShadowDrawBillboard(CScnMaruShadowNw4r* shadow, ShadowCtx* ctx, float scale);

} // extern "C"

// String / key globals referenced by the shadow setup.
extern const char* lbl_eu_8066395C; // user-data dictionary key A
extern const char* lbl_eu_80663960; // user-data dictionary key B
extern const char lbl_eu_80663968[];
extern const char lbl_eu_8066396C[];
extern const char lbl_eu_80663970[];
extern const char lbl_eu_80529658[];
extern const char lbl_eu_80529678[];
extern const char lbl_eu_80530CF0[];
extern const char lbl_eu_80530CFC[];
extern const char lbl_eu_80530D18[];
extern const char lbl_eu_80530D2C[];
extern const char lbl_eu_80530D54[];
extern const char lbl_eu_80530D68[];
extern const char lbl_eu_80530D78[];
extern const char lbl_eu_80530D94[];
extern const char lbl_eu_80530DA8[];
extern const char lbl_eu_80530DC4[];
extern const char lbl_eu_80530DD8[];
extern const char lbl_eu_80530E1C[];
extern const char lbl_eu_80530E30[];
extern const char lbl_eu_80530E74[];
extern const char lbl_eu_80530EE0[];
extern const char lbl_eu_80530F08[];

extern "C" const mlCVec3 unitY__Q22ml5CVec3;

// Extra string/data labels referenced by the shadow functions.
extern const char lbl_eu_80526300[];
extern const char lbl_eu_80526324[];
extern "C" const char lbl_eu_8056E578[];   // node-name table (char* fields @0x14/@0x18)
extern "C" const float lbl_eu_80523F78[];  // per-instance scale table

// Extra string/data labels referenced by the shadow functions.
struct MaruShadowEntry {
    u16 scaleQ; // quantized scale
    u16 nodeId; // node id/name from the model resource
};

class CScnMaruShadowNw4r {
public:
    u32 vtable;        // 0x00
    u8 field_0x04;     // 0x04
    u8 field_0x05;     // 0x05
    u8 field_0x06;     // 0x06
    u8 field_0x07;     // 0x07 -- shadow map size log2? init 0x80
    float field_0x08;  // 0x08
    float field_0x0C;  // 0x0c -- shadow inner radius
    float field_0x10;  // 0x10 -- shadow outer radius
    float field_0x14;  // 0x14
    float field_0x18;  // 0x18
    float field_0x1c;  // 0x1c
    float field_0x20;  // 0x20
    float field_0x24;  // 0x24
    u16 field_0x28;    // 0x28 flags
    u8 field_0x2a;     // 0x2a
    u8 field_0x2b;     // 0x2b
    MaruShadowEntry entries[8]; // 0x2c..0x4b
    u32 field_0x4c;    // 0x4c
    u32 field_0x50;    // 0x50
};

// Retail __dt__4CScnFvMaruShadowNw4r only frees the object when the delete
// flag is set, so the TU needs CScn as an opaque pointer type only (the
// canonical class lives in monolib/scn/CScn.hpp).
class CScn;
