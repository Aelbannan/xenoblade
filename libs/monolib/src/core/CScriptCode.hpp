#pragma once

#include "monolib/work/CWorkThread.hpp"
#include "monolib/core/CToken.hpp"

class CScriptCode : public CWorkThread {
public:
    virtual ~CScriptCode();

    void func_8043A1DC(void* pData, u32 dataSize);
    s16 func_8043A2F8(u8* pOut, u8* pIn);
    void func_8043A390(void* pData, int doSecondary);
    void func_8043A57C(void* pTarget);
    void func_8043A70C(void* pKey, void* pValue);
    BOOL wkStandbyLogout();
    static CScriptCode* create(CWorkThread* pParent);
    static CScriptCode* getInstance();

    // 0x0-0x1C4: CWorkThread base
    void* m_slot0[128];   //0x1C4: primary slot array
    s16 m_cnt0;           //0x3C4: primary count
    s16 pad_0x3C6;        //0x3C6
    void* m_slot1[128];   //0x3C8: secondary slot array
    s16 m_cnt1;           //0x5C8: secondary count
    s16 pad_0x5CA;        //0x5CA
    CToken m_token;       //0x5CC
    u32 pad_0x1734;       //0x1734
    u32 m_1738;           //0x1738
};