#pragma once

#include <types.h>

// Full object layout for C-linkage accessors
struct CActParamAnimFull {
    u8 _00[0x10];
    u8 mChildData[0x4AE]; // from +0x10 to +0x4BE
    u8 mEffByte0;          // +0x4BE
    u8 mEffByte1;          // +0x4BF
    u8 _4C0[4];            // +0x4C0: eff obj pointer
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
    void func_80053164();

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
};

