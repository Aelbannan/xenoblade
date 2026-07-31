#pragma once

#include "monolib/work/CWorkThread.hpp"

class CScriptCode : public CWorkThread {
public:
    virtual ~CScriptCode();

    void func_8043A1DC(void* pData, u32 dataSize);
    s32 func_8043A2F8(void* pOut, void* pIn);
    s32 func_8043A390(s32 flag, void* pData, s32 doSecondary);
    void func_8043A57C(void* pTarget);
    void func_8043A70C(void* pKey, void* pValue);
    BOOL wkStandbyLogout();
    static CScriptCode* create(CWorkThread* pParent);
    static CScriptCode* getInstance();
};
