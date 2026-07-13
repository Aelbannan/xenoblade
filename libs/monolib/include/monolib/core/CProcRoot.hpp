#pragma once

#include "monolib/work/CWorkThread.hpp"

class CProcRoot : public CWorkThread {
public:
    // No out-of-line ctor - retail create inlines CWorkThread + vtable init.
    ~CProcRoot();

    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    static CProcRoot* create(CWorkThread* pParent);
    static CProcRoot* getInstance();

    u8 unk1C4[4];

private:
    static const int MAX_CHILD = 32;

    static CProcRoot* spInstance;
};
