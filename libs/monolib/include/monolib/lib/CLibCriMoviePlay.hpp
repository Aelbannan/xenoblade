#pragma once

#include "monolib/work/CWorkThread.hpp"
#include "monolib/device/CDeviceVICb.hpp"
#include "monolib/util/CErrorWii.hpp"

class CLibCriMoviePlay : public CWorkThread, public CDeviceVICb, public IErrorWii {
public:
    CLibCriMoviePlay(const char* pName, CWorkThread* pParent);
    virtual ~CLibCriMoviePlay();
    virtual void wkUpdate();
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();
    void func_8045AE84();
    void func_8045B1E0();
    void OnPauseTrigger();
    void func_8045B310();

    // TODO: add fields
    void func_8045B1DC();
    void func_8045B3D4();
    void func_8045B3DC();
    void func_8045B3E4();
};

