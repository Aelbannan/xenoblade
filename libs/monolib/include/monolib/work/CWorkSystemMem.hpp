#pragma once

#include "monolib/work/CWorkThread.hpp"
#include "monolib/util.hpp"

class __declspec(novtable) CWorkSystemMem : public CWorkThread {
public:
    CWorkSystemMem(const char* pName, CWorkThread* pParent);
    virtual ~CWorkSystemMem();

    DECL_WORKTHREAD_CREATE(CWorkSystemMem);

    static mtl::ALLOC_HANDLE getHandle();

    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

private:
    //0x0: vtable
    //0x0-1c4: CWorkThread
    mtl::ALLOC_HANDLE mHandle; //0x1C4

    static const u32 REGION_SIZE = 0x1000 - sizeof(mtl::MemBlock);

    static CWorkSystemMem* spInstance;
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Retail SDA singleton (getHandle / ctor / dtor reloc name).
extern "C" CWorkSystemMem* lbl_eu_80665620[2];  // 8-byte sbss; word 0 = singleton

// Retail CWorkSystemMem vtable (0x8056BAA8) + RTTI base list
// (0x8056BB48): defined explicitly in CWorkSystemMem.cpp (blob
// monolibdata1d dissolve); retail ctor stores it explicitly.
