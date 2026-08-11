#pragma once

#include "monolib/device/CDeviceVICb.hpp"
#include "monolib/work/CWorkUtil.hpp"
#include "monolib/work/CWorkThread.hpp"
#include "monolib/util/CErrorWii.hpp"

//size: 0x1d0
class CLibCri : public CWorkThread, public CDeviceVICb, public IErrorWii {
public:
    CLibCri(const char* pName, CWorkThread* pParent);
    ~CLibCri();

    // CWorkThread virtual overrides
    virtual void wkUpdate();
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    // CDeviceVICb virtual override (thunk adjusts this -0x1C4)
    virtual void viBeginFrame();

    // IErrorWii virtual override (thunk adjusts this -0x1C8)
    virtual void errorWiiCB();

    static CLibCri* spInstance;
    static CLibCri* getInstance();

    static void func_80459830();
    static int func_8045997C(const char* filename, u32 allocHandle, int fileHandle);
    void func_80459A78();
    void func_80459A7C();
    static void func_80459A80();
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

    static inline CLibCri* create(const char* pName, CWorkThread* pParent){
        CLibCri* lib = new (CWorkThreadSystem::getWorkMem()) CLibCri(pName, pParent);
        CWorkUtil::entryWork(lib, pParent, false);
        return lib;
    }

    //0x0: vtable
    //0x0-1c4: CWorkThread
    //0x1c4-1c8: CDeviceVICb
    //0x1c8-1cc: IErrorWii
    u32 unk1D0;
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// CLibCriStreamingPlay factory function (static, 8 args)
// Called from func_8045997C with various parameters
extern "C" int func_8045B5AC(
    int fileIdOrName,
    u32 allocHandle,
    int fileHandle,
    int out2,
    int out3,
    bool hasPrefix
);

// String constant (CRI file extension marker: lbl_eu_80522FD8)
extern "C" const char lbl_eu_80522FD8[];
