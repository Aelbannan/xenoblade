#pragma once

#include <types.h>
#include "monolib/monolib_types.hpp"
#include "monolib/work/CMsgParam.hpp"
#include "revolution/GX.h"

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" { extern char lbl_eu_8056BFE4[]; }
extern "C" { extern char lbl_eu_8056BFF0[]; }
extern "C" void dispatchCmdGX__8CGXCacheFv(void* self, u32 cmd);
extern "C" void disableTexGen__8CGXCacheFv(void* self);
extern "C" void resetTevState__8CGXCacheFv(void* self);
extern "C" void* getCmdPayload__8CGXCacheFv(void* self, u32 cmd);

//size: 0x4
class IStateCache {
public:
    virtual ~IStateCache();
};

//size: 0x51c
class CGXCache : public IStateCache {
public:
    CGXCache();
    virtual ~CGXCache();
    void clearStubFunc(u32 r4);
    void bindTextureGX(GXTexObj* pTexObj, u16 r5, u16 r6);
    ml::CCol4* getClearColor();
    void func_8044B660();
    bool resetGXStateA();
    void resetMtxState();
    void setZCompareMD(int r4, int r5);
    void setZWriteMode(int r4, int r5);
    void setTevColorNo(const ml::CCol4& r4, int r5);
    void setTevColorTx(const ml::CCol4& r4, int r5);
    void setDirectColA(int r4);
    void setDirectColB(int r4);
    void setBlendState(int r4, int r5);
    void func_8044B8CC(float f1, float f2, float f3);
    u32 func_8044BD74(UNKWORD r3);

    //0x0: vtable
    //0x0-0x4: IStateCache
    CMsgParam<32> unk4;
    u8 unk4A0[0x50C - 0x4A0];
    u32 unk50C;
    u8 unk510[0xC];
};
