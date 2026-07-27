#pragma once

#include <types.h>

class CProc {
public:
    void convertToProc();
    void pssCreateView();

    // TODO: add fields
    CProc();
    virtual ~CProc();
    void pssGetRoot(CProc* pProc);
    void pssSetFocus();
    void pssDetachView(WORK_ID id);
    void wkStandbyLogin();
    void wkStandbyLogout();
};

