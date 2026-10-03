#pragma once

#include <types.h>
#include <revolution/GX.h>
#include "monolib/math/CMat34.hpp"
#include "monolib/math/CCol4.hpp"

// novtable: retail vtables stored manually (stored by ctor, not compiler)
class __declspec(novtable) CGXCache {
public:
    CGXCache();
    virtual ~CGXCache();
    void execGXCmd();
    void updateCmd2Color();
    void setBlendState(int param1, int param2);
    void updateCmd1Words();
    void setZCompareMD(int param1, int param2);
    void setZWriteMode(int param1, int param2);
    void setAlphaBlend();
    void setTevColorNo(const ml::CCol4& color, int flag);
    void setTevColorTx(const ml::CCol4& color, int flag);
    void setDirectColA(int param);
    void setDirectColB(int param);
    void cacheScissor();
    bool bindTextureGX(GXTexObj* texObj, u16 w, u16 h);
    void getClearColor();
    void updateOrthoGX();
    void setOrthoProj();
    void setPerspProj(f32 fovy, f32 znear, f32 zfar);
    void makePerspProj();
    s32 scaleRingWidth(s32 param);
    void getViewRectGX();
    void getWhiteColor();
    void disableTexGen();
    void resetMtxState();
    void resetTevState();
    void resetGXFull();
    void dispatchCmdGX();
    void getCmdPayload();
    void findRingEntry(u32 id);

    // TODO: add fields
    void clearStubFunc();
    void getSubRectPtr();
    void getAdjustFlag();
    void resetGXStateA();
};

class __declspec(novtable) IStateCache {
public:
    virtual ~IStateCache();
    IStateCache();

    // TODO: add fields
};

class __declspec(novtable) CMsgParam_32 {
public:
    CMsgParam_32();

    // TODO: add fields
    virtual ~CMsgParam_32();
};

