#pragma once

#include <types.h>
#include "monolib/work/CWorkThread.hpp"
#include "monolib/work/CWorkSystem.hpp"

// novtable: the retail vtable (lbl_eu_8056CDA0) + RTTI live in this TU's
// split range (monolibdata2 dissolve) and are defined explicitly in CLib.cpp;
// __declspec(novtable) stops MWCC from also emitting a duplicate __vt__4CLib
// + RTTI blocks. The ctor assigns lbl_eu_8056CDA0 explicitly (CToken recipe).
class __declspec(novtable) CLib : public CWorkThread {
public:
    CLib(const char* pName, CWorkThread* pParent);
    ~CLib();

    DECL_WORKTHREAD_CREATE(CLib);

    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    static CLib* getInstance();

    static bool isInitialized();
    void createLibs();
    static CLib* create();

    //0x0: vtable
    //0x0-1c4: CWorkThread
    u32 unk1C4;
private:
    static const int MAX_CHILD = 8;

    static CLib* spInstance;
};
