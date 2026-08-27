#pragma once

#include <types.h>
#include <revolution/GX.h>
#include <revolution/gx/GXVert.h>

// Foreign .sdata/.sdata2 float constants referenced by this TU.
extern float lbl_eu_80663964;
extern float lbl_eu_80658638[3];
extern float lbl_eu_806658E0;
extern const float lbl_eu_8066A940;
extern const float lbl_eu_8066A944;
extern const float lbl_eu_8066A948;
extern const float lbl_eu_8066A994;
extern const float lbl_eu_8066A9BC;
extern const float lbl_eu_8066A9C0;
extern const float lbl_eu_8066A9C4;
extern const float lbl_eu_8066A9C8;
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
    void* field_0x14AC;
};

class CScnMaruShadowNw4r;

// nw4r g3d / db imports used by this TU.
extern "C" {
extern int GetResNodeNumEntries__Q34nw4r3g3d6ResMdlCFv(nw4r::g3d::ResMdl mdl);
extern nw4r::g3d::ResNode GetResNode__Q34nw4r3g3d6ResMdlCFUl(nw4r::g3d::ResMdl mdl, unsigned long index);
extern nw4r::g3d::ResNode GetResNode__Q34nw4r3g3d6ResMdlCFPCc(nw4r::g3d::ResMdl mdl, const char* name);
extern void* GetResUserData__Q34nw4r3g3d7ResNodeFv(nw4r::g3d::ResNode node);
extern void* __vc__Q34nw4r3g3d6ResDicCFPCc(nw4r::g3d::ResDic dic, const char* key);
extern void Panic__Q24nw4r2dbFPCciPCce(const char* file, int line, const char* fmt, ...);
extern void Warning__Q24nw4r2dbFPCciPCce(const char* file, int line, const char* fmt, ...);

// monolib helpers called from the maru-shadow draw path.
extern void* func_8049621C(u32 unk, int id);
extern void* func_804961D4(u32 unk, int id);
extern void getScnHandle();
extern int func_804BCC10();
extern bool func_804BE398(mlCVec3* out, float x, float y, int a, int b);
extern void func_804BE3E0(const void* cb, void* mtx, u32 unk, int flag);
extern bool func_804BE4B4(mlCVec3* v, int flag);
extern bool func_804BE4E0(mlCVec3* v, int flag);
extern void* func_804BE50C(int idx);
extern void* func_8048315C(ShadowCtx* ctx);
extern void func_8048E2F8(CScnMaruShadowNw4r* shadow, ShadowCtx* ctx, float scale);
} // extern "C"

// Foreign .sdata2 constants used by the draw path.
extern const float lbl_eu_8066A990;
extern const float lbl_eu_8066A998;
extern const float lbl_eu_8066A99C;
extern const float lbl_eu_8066A9A0;
extern const float lbl_eu_8066A9B8;
extern const double lbl_eu_8066A950;
extern const double lbl_eu_8066A9A8;
extern const double lbl_eu_8066A9B0;

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
extern const float lbl_eu_8066A9D0;

extern "C" const mlCVec3 unitY__Q22ml5CVec3;

// Extra string/data labels referenced by the shadow functions.
extern "C" const char lbl_eu_80526300[];
extern "C" const char lbl_eu_80526324[];
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

// Non-polymorphic scene base whose storage is managed externally; the
// destructor only releases the object memory when the delete flag is set.
struct CScn {
    u8 storage[4];
};
