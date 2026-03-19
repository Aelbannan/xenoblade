#pragma once

#include "monolib/work/CWorkThread.hpp"
#include "monolib/util.hpp"

class CWorkSystem : public CWorkThread {
public:
    typedef void (*ExitFunc)();

public:
    CWorkSystem(const char *pName, CWorkThread *pParent);
    virtual ~CWorkSystem();

    static CWorkSystem* getInstance();
    static bool isOff();
    static mtl::ALLOC_HANDLE getMem();
    static bool isPowerOff();
    static bool isReset();
    static void setSaveLoadInvalidReset(bool state);

    virtual void wkUpdate();
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    static CWorkSystem* create();
    DECL_WORKTHREAD_CREATE(CWorkSystem);

    static void setExitFunc(ExitFunc func);
    static void callExitFunc();

private:
    //0x0: vtable
    //0x0-1c4: CWorkThread
    mtl::ALLOC_HANDLE mMemHandle; //0x1C4
    bool mPowerOff; //0x1C8
    bool mReset; //0x1C9
    bool mSaveLoadInvalidReset; //0x1CA
    u8 unk1CB[0x1D0 - 0x1CB];

    static CWorkSystem* spInstance;
    static ExitFunc sExitFunc;
};

//Reset handling functions. Due to string pooling, these had to have been defined outside of a class as static functions.

/* TODO: Ideally this wouldn't need to be a macro, but for files using O4,s (CWorkSystem.cpp), if a function
ends up calling the same function twice, which happens in CWorkSystem::wkUpdate, it refuses to inline it. */
#define prepareReset(){          \
    CWorkSystem::callExitFunc(); \
                                 \
    VISetBlack(VI_TRUE);         \
    VIFlush();                   \
                                 \
    VIWaitForRetrace();          \
    VIWaitForRetrace();          \
    VIWaitForRetrace();          \
    VIWaitForRetrace();          \
    VIWaitForRetrace();          \
    VIWaitForRetrace();          \
}                           

static inline void resetGame(bool direct){
    if(!direct){
        prepareReset();
    }

    //Restart
    OSReport("exit wii reset\n");
    OSRestart(0);
}

static inline void shutdownGame(bool direct){
    if(!direct){
        prepareReset();
    }

    //Restart
    OSReport("exit wii power off\n");
    OSShutdownSystem();
}


static inline void returnToWiiMenu(bool direct){
    if(!direct){
        prepareReset();
    }

    //Restart
    OSReport("exit wii menu\n");
    OSShutdownSystem();
}
