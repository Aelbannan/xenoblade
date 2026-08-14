#pragma once

#include <types.h>

#include "kyoshin/action/CActParamData.hpp"

// Full object layout for C-linkage accessors
struct CActParamAnimFull {
    u8 _00[0x10];
    u8 mChildData[0x4AE]; // from +0x10 to +0x4BE
    u8 mEffByte0;          // +0x4BE
    u8 mEffByte1;          // +0x4BF
    u8 _4C0[4];            // +0x4C0: eff obj pointer
};

// Cast-only view of the anim-state region (+0x394..+0x440) plus the flags
// word at +0x0C. Used by the C-linkage state helpers below.
struct CActParamAnimStateView {
    u8 _pad_00[0x0C];           // +0x00..+0x0B
    u32 field0C;                // +0x0C: flags
    u8 _pad_10[0x394 - 0x10];   // +0x10..+0x393
    f32 field394;               // +0x394: anim speed
    u8 _pad_398[0x3A0 - 0x398]; // +0x398..+0x39F
    u8* object3A0;              // +0x3A0: attached sub-object
    u32 state3A4;               // +0x3A4: sub-object state
    u32 field3A8;               // +0x3A8
    u32 field3AC;               // +0x3AC
    u32 field3B0;               // +0x3B0
    u32 field3B4;               // +0x3B4
    u32 field3B8;               // +0x3B8
    u32 field3BC;               // +0x3BC
    u8 _pad_3C0[4];             // +0x3C0
    f32 field3C4;               // +0x3C4
    u8 _pad_3C8[0x430 - 0x3C8]; // +0x3C8..+0x42F
    f32 field430;               // +0x430
    f32 field434;               // +0x434
    u8 _pad_438[0x440 - 0x438]; // +0x438..+0x43F
    f32 field440;               // +0x440
    u8 _pad_444[0x488 - 0x444]; // +0x444..+0x487
    f32 field488;               // +0x488: last anim value (stored as f32)
};

// Raw 32-bit word view of a position/scale triple. func_8004B354/B40C copy
// these as plain words (lwz/stw in retail), while field488 in the state view
// reinterprets the y word as f32.
struct CActParamAnimData3 {
    u32 x;
    u32 y;
    u32 z;
};

// Cast-only interface for the owner object at +0x08: MWCC places the first
// user virtual at vt+0x08 (two RTTI entries at vt+0x00/0x04), so the 4th
// virtual lands at vt+0x14, the slot func_8004B4A4 dispatches through.
struct CActParamAnimOwnerIf {
    virtual int v0();
    virtual int v1();
    virtual int v2();
    virtual int v3();
    u8 _pad[0x10];  // +0x04..+0x13
    f32 field14;    // +0x14
};

class CActParamAnim {
public:
    CActParamAnim();
    virtual ~CActParamAnim();
    void func_8004B114();
    void func_8004D074();
    void func_8004D194();
    void func_8004D2F8();
    void func_8004D4AC();
    void func_8004D650();
    void func_8004D7EC();
    void func_8004D950();
    void func_8004DAE0();
    void func_8004DDD0();
    void func_8004DF9C();
    void func_8004E168();
    void func_8004E334();
    void func_8004E500();
    void func_8004E694();
    void func_8004E828();
    void func_8004E9EC();
    void func_8004ECF4() const;
    void func_8004F1E4();
    void func_8004F334();
    void func_8004F484();
    void func_8004F5FC();
    void func_8004F884();
    void func_8004FAB4();
    void func_8004FCE0();
    void func_8004FE58();
    void func_8004FFBC();
    void func_80050744();
    void func_80050890();
    void func_80050C50();
    void func_80050DB0();
    void func_80050F5C();
    void func_800510FC();
    void func_800512A8();
    void func_80051448();
    void func_80051584();
    void func_800516C0();
    void func_800517FC();
    void func_8005194C();
    void func_80052934();

    // Converted free functions → members
    void* getChild();
    float getBlendWeight();
    int checkFlag(int mask);
    void* getOwner();
    float getScale();
    void updateSubAnim();
    void setOwner(int val);
    float getAnimSpeed() const;
    void copyVec3To3C0(const float* src);
    void setVec3Y(float val);
    int checkAnimFlag();
    float func_8004B854();
    float func_8004B86C();
    void callObjUpdate();
    float func_8004B898();
    int callObjFunc(void* param);
    void* getNextChainObj();
    void setBlendFlag(int param);
    void* getModelObj();
    int checkRenderFlag();
    void setAnimCounter(int value);
    void* getEffObj();
    void clearEffObj();
    void func_8004C5E8();
    void resetVec3Y();
    void setActiveFlag(s32 param);
    void startAnimA();
    void startAnimA2();
    void startAnimB();
    void startAnimC();
    void stopAnim();
    u32 checkFlag13();
    float getParamFloat();
    float getField10();
    u32 checkFlag16();
    u32 testAndClearFlag18();
    void mulVec3Y(float param_2);
    int getChainInt();
    bool func_80053990();
    bool func_80053998();
    bool func_800539A0();
    bool func_800539A8();
    bool func_800539B0();
    bool func_800539B8();
    bool func_800539C0();
    bool func_800539C8();
    bool func_800539D0();
    bool func_800539D8();

private:
    u8 _pad_04[0x08];      // +0x04..+0x0B
    u32 mField0C;          // +0x0C: zero-initialized in ctor
    CActParamData mChildData; // +0x10: embedded CActParamData sub-object
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void __dl__FPv(void* object);
extern "C" void* func_8048315C(void* object);
extern "C" void func_8004B9D4(CActParamAnim* self, u32, u32, s32, u32);
extern "C" void func_80053B24(void* a, void* b, void* c, void* d, void* e);
extern "C" bool func_80055B88(void* data);
extern "C" void func_80055AC4(void* data);
extern "C" u32 func_804BD94C(void* a, void* b, u32 c, u32 d, u32 e, u32 f,
                              f32 g, f32 h, f32 i, f32 j, f32 k);
extern "C" {
extern float lbl_eu_80665ECC;
}
extern "C" u8 lbl_eu_80663D4C;
// sdata2 float constants used by the anim start/setup helpers (f32, loaded
// via lfs in retail).
extern float lbl_eu_80665EB8;
extern float lbl_eu_80665EA0;
extern float lbl_eu_80665ED8; // fidx scale (func_800526C0 / func_8004CC68)
extern float lbl_eu_80665F00; // half-angle (func_800526C0)
extern float lbl_eu_80665F18;
extern float lbl_eu_80665F3C;
extern float lbl_eu_80665F6C;
extern float lbl_eu_80665F70;
extern float lbl_eu_8066A1F8; // pi (angle wrap, func_8004BC28)
extern float lbl_eu_8066A1FC; // two*pi (angle wrap, func_8004BC28)

// nw4r debug strings used by the FSqrt-style assert (file/msg pair).
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

