#pragma once

#include <types.h>

class CDesktop {
public:
    void wkUpdate();
    void wkRender();
    void wkStandbyLogin();
    void wkStandbyLogout();
    void getCurrentProc();
    void OnFileEvent();
    void WorkEvent3();

    // TODO: add fields
    CDesktop();
    virtual ~CDesktop();
    void getInstance();
    void getView();
    void getException();
    void wkRenderAfter();
    void entryTable(DesktopIcon* pIcon, bool state);
    void setAppException(int r3);
};

