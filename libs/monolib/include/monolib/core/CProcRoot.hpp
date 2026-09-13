#pragma once

#include "monolib/work/CWorkThread.hpp"

class __declspec(novtable) CProcRoot : public CWorkThread {
public:
    // No out-of-line ctor - retail create inlines CWorkThread + vtable init.
    ~CProcRoot();

    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    static CProcRoot* create(CWorkThread* pParent);
    static CProcRoot* getInstance();

    u32 unk1C4; // 0x1C4

private:
    static const int MAX_CHILD = 32;

    static CProcRoot* spInstance;
};
