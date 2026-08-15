#pragma once

#include <types.h>

#include "monolib/work/CWorkThread.hpp"
#include "monolib/work/CWorkUtil.hpp"

class __declspec(novtable) CWorkControl : public CWorkThread{
public:
    static CWorkControl* getInstance();
    static CWorkControl* create(CWorkThread* pParent);
    static CWorkControl* create(const char* pName, CWorkThread* pParent);

    DECOMP_DONT_INLINE static bool setFlowSetup();
    static void pause(bool paused);
    static bool hasFlow();

    virtual bool wkStandbyLogin();  //0x94
    virtual bool wkStandbyLogout(); //0x98

private:
    static const int MAX_CHILD = 32;

private:
    CWorkControl(const char* pName, CWorkThread* pParent);
    virtual ~CWorkControl();

private:
    char unk1C8[4];

private:
    static CWorkControl* spInstance;
};
