#pragma once

#include <types.h>

class CLibCri {
public:
    CLibCri(const char* pName, void* pParent);
    virtual ~CLibCri();

    // CWorkThread virtual overrides
    virtual void wkUpdate();                        // 0x88
    virtual bool wkStandbyLogin();                  // 0x94
    virtual bool wkStandbyLogout();                 // 0x98

    // CDeviceVICb virtual overrides (thunks adjust this -0x1C4)
    virtual void viBeginFrame();                    // -> func_80459AD8

    // IErrorWii virtual override (thunk adjusts this -0x1C8)
    virtual void errorWiiCB();                      // -> func_80459C74

    void func_80459830();
    void func_8045997C(const char* filename, u32 allocHandle, int fileHandle);
    void func_80459A78();
    void func_80459A7C();
    void func_80459A80();
    void func_80459A84();
    void func_80459A88();
    void func_80459A8C();
    void func_80459A90();
    void func_80459A94();
    void func_80459A98();
    void func_80459A9C();
    void func_80459AA0();
    void func_80459AA4();
    void func_80459AA8();
    void func_80459AAC();
    void func_80459AB0();
    void func_80459AC0();
    void func_80459AC4();
    void func_80459AC8();
    void func_80459ACC();
    void func_80459AD0();
    void func_80459AD8();
    void func_80459C74();

    static CLibCri* getInstance();

    // Thunk implementations (virtual override implementations)
    // func_80459C78 = viBeginFrame thunk (adjusts this -0x1C4)
    // func_80459C80 = ~CDeviceVICb dtor thunk (adjusts this -0x1C4)
    // func_80459C88 = errorWiiCB thunk (adjusts this -0x1C8)
    // func_80459C90 = ~IErrorWii dtor thunk (adjusts this -0x1C8)
};

