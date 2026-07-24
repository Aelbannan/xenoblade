#pragma once

#include "monolib/lib/CLibCri.hpp"

//size: 0x540 (estimated based on CLibCri + streaming fields)
class CLibCriStreamingPlay : public CLibCri {
public:
    CLibCriStreamingPlay(const char* pName, CWorkThread* pParent);
    virtual ~CLibCriStreamingPlay();

    // CDeviceVICb virtual overrides (thunks adjust this from CDeviceVICb subobject at 0x1c4)
    virtual void viBeforeDrawDone();
    virtual void viAfterDrawDone();
    virtual void viBeginFrame();

    // Internal methods
    void func_8045CF30();
    void func_8045CFDC();
    void func_8045D03C();

    // Virtual thunks (FULL_MATCH - adjust this from CDeviceVICb subobject)
    void func_8045D140();
    void func_8045D148();

    // Other methods (stubs)
    static void func_8045B5AC();
    static void func_8045B970();
    static void func_8045BAB0();
    static void func_8045BBA0();
    static void func_8045BC4C();
    static void func_8045C67C();
    static void func_8045C700();
    static void func_8045C8B0();
    static void func_8045CA4C();
    static void func_8045CCFC();

    // Callback stubs
    virtual bool wkStandbyLogin();
    virtual void OnPauseTrigger(bool paused);
};
