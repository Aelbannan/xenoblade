#pragma once

#include <types.h>

class CScnRoot {
public:
    CScnRoot(void* mgr);
    ~CScnRoot();

    // 0x00: vtable
    // 0x04: mpMgr
    // 0x08: mReserved
private:
    void* mpMgr;      // 0x04
    u32 mReserved;    // 0x08
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void __dt__12CScnRootNw4rFv(void* self, int deleteFlag);
