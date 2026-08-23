#pragma once

#include <types.h>
#include "monolib/math/CVec3.hpp"

// Environment-light-control object layout used by CTaskEnvironment::Init
// (reached through the scene manager slot at +0x7C; retail monolib
// CScnEnvLgtCtrl). Only the flag word Init touches is declared.
struct CTaskEnvLgtCtrlAccess {
    u32 field_0;   // +0x00 flag word (Init ORs 0x80)
};

// World/clock source object reached through CTaskEnvScene +0x78 (and through
// the scene-process argument of the constructor). Its clamped time fields are
// copied into CTaskEnvironment at construction and in func_80058844.
struct CTaskEnvWorld {
    u8 field_0x00[0x54];
    f32 field_0x54;   // +0x54
    f32 field_0x58;
    u8 pad5C[0x64 - 0x5C];
    u32 field_0x64;   // +0x64 day-of-month
    u32 field_0x68;   // +0x68 hour
    u32 field_0x6C;   // +0x6C minute
    u32 field_0x70;   // +0x70
};

// Scene manager sub-object (CTaskEnvScene +0x5C). The light-setup helper
// registers lights into named slots and toggles the +0x19 flag.
struct CTaskEnvScnMgr {
    u8 pad00[0xC];
    u8 field_0x0C[0x19 - 0x0C];  // slot A (base address passed to scn helpers)
    u8 field_0x19;               // flag set after the first light registration
    u8 pad1A[0x2C - 0x1A];
    u8 field_0x2C[0x20];         // slot B
    u8 field_0x4C[0x20];         // slot C
    u8 field_0x6C[0x20];         // slot D
};

// Scene object held by CTaskEnvironment at +0x54 (retail CScn manager layout;
// the env-light-control slot sits at +0x7C).
struct CTaskEnvScene {
    u8 field_0x00[0x54];
    f32 field_0x54;                       // +0x54
    f32 field_0x58;                       // +0x58
    CTaskEnvScnMgr* field_0x5C;           // +0x5C scene manager sub-object
    u8 pad60[0x64 - 0x60];
    u32 field_0x64;                       // +0x64 day-of-month
    u32 field_0x68;                       // +0x68 hour
    u32 field_0x6C;                       // +0x6C minute
    u32 field_0x70;                       // +0x70
    u8 pad74[0x78 - 0x74];
    CTaskEnvWorld* field_0x78;            // +0x78 world/clock object
    CTaskEnvLgtCtrlAccess* mEnvLgtCtrl;   // +0x7C
};

// Opaque monolib light-group objects created by func_80058844.
class CTimeLightGrp;
class CVirtualLightObj;

// Object pointed to by CTaskEnvironment +0xE0/+0xE4 (Term calls its vtable
// slot-1 method with 1; retail r12 indirect dispatch).
struct CTaskEnvUnk {
    virtual void vfUnk1(int x);
};

// Global environment manager (retail lbl_eu_80663D58). Recovered layout:
// +0x68 record index, +0x6C record array (60-byte records).
struct CTaskEnvGlobal {
    u8 field_0x00[0x68];
    u32 field_0x68;   // +0x68 record index
    u8* field_0x6C;   // +0x6C record array (60-byte records)
    u8 field_0x70[0x7C - 0x70];
    u32 field_0x7C;   // +0x7C
};

// C-linkage import (retail symbol): env-light-control enable (monolib scn).
extern "C" void func_804C1094(CTaskEnvLgtCtrlAccess* ctrl, int flag);

// Time-of-day record: seconds as float at +0x70, minute/hour/day/month carry
// chain (+0x6C/+0x68/+0x64/+0x60) and a total counter at +0x5C. Written by
// func_800599E0 / func_80059A48, read by func_800587F8.
struct CTaskEnvTime {
    u8 field_0x00[0x5C];
    u32 total;    // +0x5C accumulates month carries
    u32 month;    // +0x60 (mod 12)
    u32 day;      // +0x64 (mod 30)
    s32 hour;     // +0x68
    s32 minute;   // +0x6C
    f32 second;   // +0x70
};

// 3x3 rotation matrix (nine contiguous floats): built by func_80059610,
// applied to vectors by func_800598A8.
struct CTaskEnvRotMtx {
    float m00, m01, m02;
    float m10, m11, m12;
    float m20, m21, m22;
};

// sdata2 constants shared by the clock/rotation/light helpers.
extern const float lbl_eu_80665FA0;
extern const float lbl_eu_80665FB4;
extern const float lbl_eu_80665FCC;
extern const float lbl_eu_80665FD0;
extern const float lbl_eu_80665FD4;
extern const float lbl_eu_80665FD8;
extern const float lbl_eu_80665FDC;
extern const float lbl_eu_80665FE0;
extern const float lbl_eu_80665FE4;
extern const float lbl_eu_80665FE8;
extern const float lbl_eu_80665FA4;
extern const float lbl_eu_80665FA8;
extern const float lbl_eu_80665FB0;
extern const float lbl_eu_80665FAC;
extern const float lbl_eu_80665FC0;
extern const float lbl_eu_80665FC4;
extern const float lbl_eu_80665FC8;
extern const float lbl_eu_80665FF8;
extern const float lbl_eu_80665FFC;
extern const float lbl_eu_80666000;
extern const float lbl_eu_80666004;
extern const float lbl_eu_80666008;
extern const float lbl_eu_8066600C;
extern const float lbl_eu_80666010;
extern const float lbl_eu_80666014;
extern const float lbl_eu_80666018;
extern const float lbl_eu_8066601C;
extern const float lbl_eu_80666020;
extern const float lbl_eu_80666024;
extern const float lbl_eu_80666028;
extern const float lbl_eu_8066602C;
extern const float lbl_eu_80666030;
extern const float lbl_eu_80665FEC;

// .data float constant used by Move's sun-angle computation.
extern const float lbl_eu_8066A1FC;

// Imports used only by CTaskEnvironment::Move (retail unmangled symbols).
class CVirtualLightObj2;
extern "C" int func_8007E1B4__Q22cf13CfGameManagerFv();
extern "C" void func_80086D98__Q22cf13CfGameManagerFv(unsigned short* outA,
                                                       unsigned short* outB);
extern "C" unsigned int func_80086DA8__Q22cf13CfGameManagerFv();
extern "C" float func_80496288(CTaskEnvScene* scene);
extern "C" void func_804C123C(CTaskEnvLgtCtrlAccess* ctrl, int flag);
extern "C" void func_804C1270(CTaskEnvLgtCtrlAccess* ctrl, int flag);
extern "C" void func_804C1600(CTaskEnvLgtCtrlAccess* ctrl, void* arg);
extern "C" void* func_8004B3F0(void* dst, const void* src);
extern "C" void* func_8004B60C(ml::CVec3* out, float x, float y, float z);
extern "C" float func_8004CC74(float angle);
extern "C" void func_8005A374(CTimeLightGrp* grp);
extern "C" void func_8049E374(u8* self, float a, float b);
extern "C" void func_8049E350(u8* self, const void* src);
extern "C" void func_8049347C(CVirtualLightObj* self, const ml::CVec4* src,
                              f32 value);

// Static-init table filled by sinit_80059D60 (160 bytes of floats) and the
// ambient-light data tables read by func_80058844.
extern float lbl_eu_80570830[0xA0 / sizeof(float)];
extern const float lbl_eu_80570860[4];
extern const u8 lbl_eu_805708C0[];

// Retail CTaskEnvironment vtables (.data): interim CTask<CTaskEnvironment>
// vtable written right after the raw CProcess base ctor, then the final one.
extern const u8 lbl_eu_805263D8[];
extern const u8 lbl_eu_80526390[];

// Global null pointer-to-member-function constant (3 words).
extern u32 __ptmf_null[3];

// Raw CProcess base ctor (CProcess is abstract; the out-of-line ctor entry
// point calls it directly).
extern "C" void __ct__8CProcessFv(CProcess* self);

// C-linkage imports used by the environment-light setup (retail symbols).
extern "C" void func_804923F8(CTaskEnvScnMgr* mgr, int flag);
extern "C" void* func_80496004(void* scene);
extern "C" void __ct__CTimeLightGrp(CTimeLightGrp* self, void* parent);
extern "C" CVirtualLightObj* func_804930BC(CTaskEnvScnMgr* mgr, const u8* slot,
                                           const void* color);
extern "C" CVirtualLightObj* func_804933AC(CTaskEnvScnMgr* mgr, const u8* slot,
                                           const void* color, const void* dir);
extern "C" void func_8005A2F0(CTimeLightGrp* grp, CVirtualLightObj* obj);

// u32 word-pair / f64 view used to build the 0x43300000-prefixed int->float
// conversion pattern by hand so the pool reloc names the retail constant
// instead of an MWCC-synthesised @N entry (CfTFile.cpp convention).
union F64Conv {
    u32 w[2];
    double d;
};

// Retail global manager pointer (unmangled at global scope - no extern "C").
extern CTaskEnvGlobal* lbl_eu_80663D58;

// CRTP task base (canonical monolib template). CTaskEnvironment derives from
// CTTask<CTaskEnvironment>; the out-of-line Move/Draw/dtor specializations are
// emitted in the unit cpp, and the derived Move/Draw overrides live here.
#include "monolib/work/CTTask.hpp"

class CTaskEnvironment : public CTTask<CTaskEnvironment> {
public:
    // Retail keeps __ct__CTaskEnvironment out-of-line; declared/defined in
    // the cpp as the retail C-linkage entry point.
    CTaskEnvironment(CProcess* pArg);
    virtual ~CTaskEnvironment();
    void Init();
    void Term();
    void Move();
    void Draw();

    /* 0x54 */ CTaskEnvScene* mScene;
    /* 0x58 */ u8 field_0x58;
    /* 0x5C */ u32 field_0x5C;      // ctor: 2000
    /* 0x60 */ u32 field_0x60;
    /* 0x64 */ u32 field_0x64;
    /* 0x68 */ u32 field_0x68;      // ctor: 0x10
    /* 0x6C */ u32 field_0x6C;
    /* 0x70 */ f32 field_0x70;
    /* 0x74 */ f32 field_0x74;
    /* 0x78 */ f32 field_0x78;
    /* 0x7C */ f32 field_0x7C;
    /* 0x80 */ f32 field_0x80;
    /* 0x84 */ f32 field_0x84;
    /* 0x88 */ f32 field_0x88;
    /* 0x8C */ f32 field_0x8C;
    /* 0x90 */ u32 field_0x90;      // world clock copy: day
    /* 0x94 */ u32 field_0x94;      // world clock copy: hour
    /* 0x98 */ u32 field_0x98;      // world clock copy: minute
    /* 0x9C */ u32 field_0x9C;      // world clock copy: +0x70
    /* 0xA0 */ f32 field_0xA0;
    /* 0xA4 */ f32 field_0xA4;
    /* 0xA8 */ u8 field_0xA8;
    /* 0xAC */ CVirtualLightObj* mLgtAC;  // sun colour light
    /* 0xB0 */ CVirtualLightObj* mLgtB0;  // ambient
    /* 0xB4 */ u32 mPadB4;
    /* 0xB8 */ CVirtualLightObj* mLgtB8;
    /* 0xBC */ CVirtualLightObj* mLgtBC;
    /* 0xC0 */ CVirtualLightObj* mLgtC0;
    /* 0xC4 */ CVirtualLightObj* mLgtC4;
    /* 0xC8 */ CVirtualLightObj* mLgtC8;
    /* 0xCC */ u32 mPadCC;
    /* 0xD0 */ u32 mPadD0;
    /* 0xD4 */ CVirtualLightObj* mLgtD4;
    /* 0xD8 */ CVirtualLightObj* mLgtD8;
    /* 0xDC */ CVirtualLightObj* mLgtDC;
    /* 0xE0 */ void* mUnkE0;   // first CTimeLightGrp (Term calls its vtable slot-1 method)
    /* 0xE4 */ void* mUnkE4;   // second CTimeLightGrp (Term calls its vtable slot-1 method)
    /* 0xE8 */ u8 field_0xE8;
    /* 0xE9 */ u8 field_0xE9;
    /* 0xEA */ u8 field_0xEA;
};

// Clock helper: hours = minute/60 + second/3600 (c = sdata2 rate constant).
extern "C" float func_800587F8(CTaskEnvTime* time);
// Matrix * vector (row-major 3x3).
extern "C" void func_800598A8(ml::CVec3* out, const CTaskEnvRotMtx* m,
                              const ml::CVec3* v);
// Set clock time; hour clamped to 0 when >= 24, minute to 0 when >= 60,
// seconds clamped against the sdata2 limit constant.
extern "C" void func_800599E0(CTaskEnvTime* time, int hour, int minute,
                              int second);
// Allocate (0xEC bytes from the work-thread heap), construct and register a
// CTaskEnvironment under 'parent'.
CTaskEnvironment* func_80059C58(CProcess* parent, CProcess* arg);
// Build a Y-axis rotation matrix from 'angle' (scaled into fidx units);
// returns the matrix pointer.
extern "C" CTaskEnvRotMtx* func_80059610(CTaskEnvRotMtx* mtx, float angle);
// Environment-light setup (retail unmangled C-linkage entry point): builds
// the two CTimeLightGrp groups, creates the ambient/sun/fog light objects from
// sdata tables and copies the world clock.
extern "C" void func_80058844(CTaskEnvironment* self);
// Advance the clock record by 'delta' seconds with minute/hour/day/month
// carries (Euclidean modulo so negative deltas wrap upward).
extern "C" void func_80059A48(CTaskEnvTime* time, float delta);
// Rotate the three row vectors of 'mtx' about the X axis by 'angle'
// (fidx units scaled by the FEC constant), writing the product back.
extern "C" void func_800596B8(CTaskEnvRotMtx* mtx, float angle);

// Flat-name accessors shared with Move (retail unmangled symbols).
extern "C" u32 getSubField78(void* self);
extern "C" void* func_8005870C(void* self);
extern "C" float func_8005871C(void* self);
extern "C" float func_80058724(void* self);
extern "C" void nopFunc();
