#pragma once

#include "monolib/work/CWorkThread.hpp"

class CScriptCode : public CWorkThread {
public:
    CScriptCode(const char* pName, CWorkThread* pParent);

    static CScriptCode* create(CWorkThread* pParent);

    static CScriptCode* getInstance();
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" long func_8043A2F8__11CScriptCodeFv(void* self, void* pOut, void* pIn);
extern "C" void func_8043A390__11CScriptCodeFv(void* self, void* pData, int doSecondary);
