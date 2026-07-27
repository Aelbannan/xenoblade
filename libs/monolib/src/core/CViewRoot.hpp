#pragma once

#include <types.h>
#include "monolib/core/CDesktop.hpp"
#include "monolib/core/CProc.hpp"
#include "monolib/core/CDesktop.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/device/CGXCache.hpp"
#include "monolib/work/CWorkUtil.hpp"
#include "monolib/work/CMsgParam.hpp"

class CViewRoot {
public:
    virtual ~CViewRoot();
    void isInitialized();
    void invalidCurrent();
    void func_80442B54();
    void func_80442C68();
    void func_80442DA8();
    void getRootView();
    void wkStandbyLogin();
    void wkStandbyLogout();

    // TODO: add fields
    void getInstance();
    void getCurrent();
    void destroyProc(CProc* proc);
    void isCurrentChild(const CView* view, const CView* current);
    void isCurrent(const CView* view);
};

class CView {
public:
    void convertToView();

    // TODO: add fields
};

