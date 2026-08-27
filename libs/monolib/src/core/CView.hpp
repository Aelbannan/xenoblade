#pragma once

#include <types.h>
#include "monolib/core/CViewRectData.hpp"
#include "monolib/work/CWorkThread.hpp"

class CView {
public:
    CView();
    virtual ~CView();
    void CView_UnkVirtualFunc7();
    void updateViewRect();
    void checkSplitActive();
    void CView_UnkVirtualFunc3();
    void enqueueContextMsg();
    void CView_UnkVirtualFunc4();
    void updateViewState();
    void func_8043E46C();
    void func_8043E46C();
    void clipRectToView();
    void adjustRectForView();
    void applyRectToView();
    void func_8043E928();
    void func_8043EA88();
    void wkStandbyLogin();
    void wkStandbyLogout();

    // TODO: add fields
    void setDefaultFrameColor(const ml::CCol4& color);
    void func_8043CAFC();
    void setDisp(bool r4, bool r5);
    void getSplitFrameFlag();
    void getSplitLine();
    void setSplitLine(s16 line);
    void hasCurrent() const;
    void CView_UnkVirtualFunc9();
    void CView_UnkVirtualFunc8();
    void detachRenderWork(CWorkThread* pThread);
    void wkUpdate();
    void getCurrentView();
    void CView_UnkVirtualFunc6();
    void CView_UnkVirtualFunc5();
    void CView_UnkVirtualFunc1();
};

class CViewFrame {
public:
    virtual ~CViewFrame();

    // TODO: add fields
};

