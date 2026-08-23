#pragma once

#include <types.h>

#include "monolib/work/CWorkThread.hpp"
#include "monolib/core/CToken.hpp"

class CCmdDispatcher;

// Entry stored in the CScriptCode slot arrays: `key` is the registered
// command string, `handler` an optional dispatcher invoked when a script
// command segment matches the key.
struct CScriptCodeEntry {
    const char* key;               //0x0
    CCmdDispatcher* handler;       //0x4
};

//size: 0x1740
//
// novtable: the retail vtable/RTTI live in the blob range owned by
// CScriptCode.cpp (blob monolibdata1 dissolve) and are defined explicitly
// there, so the compiler must not emit a duplicate vtable + RTTI chain.
class __declspec(novtable) CScriptCode : public CWorkThread {
public:
    virtual ~CScriptCode();
    virtual bool wkStandbyLogout();

    static CScriptCode* create(CWorkThread* pParent);
    static CScriptCode* getInstance();

    CScriptCodeEntry* m_slot0[128];   //0x1C4: primary slot array
    s16 m_cnt0;                       //0x3C4: primary count
    s16 pad_0x3C6;                    //0x3C6
    CScriptCodeEntry* m_slot1[128];   //0x3C8: secondary slot array
    s16 m_cnt1;                       //0x5C8: secondary count
    s16 pad_0x5CA;                    //0x5CA
    CToken m_token;                   //0x5CC
    u32 field_0x1734;                 //0x1734
    u32 field_0x1738;                 //0x1738: last matched primary entry
};

// The script-code parser routines have retail symbols mangled as zero-
// argument members (`...__11CScriptCodeFv`) even though they receive extra
// arguments in registers. That shape cannot be expressed as a C++ member
// declaration, so they are declared as free functions carrying their exact
// retail symbol names.
s16 func_8043A2F8__11CScriptCodeFv(CScriptCode* self, u8* pOut, u8* pIn);
int func_8043A390__11CScriptCodeFv(CCmdDispatcher* self, const char* pData, int doSecondary);
void func_8043A1DC__11CScriptCodeFv(CScriptCode* self, u8* pData, u32 dataSize);
void func_8043A57C__11CScriptCodeFv(const char* pKey, CCmdDispatcher* pTarget);
void func_8043A70C__11CScriptCodeFv(const char* pKey, CCmdDispatcher* pValue);
