#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"
#include "kyoshin/IUIWindow.hpp"
#include "monolib/work/IWorkEvent.hpp"

/*
  Collision-6 check-bat process. Singleton managed by CCol6System.
  Inherits CProcess for work-thread lifecycle (Init/Term/Move/Draw).
  Size 0x74 bytes.
*/
class CCol6CheckBat : public IUIWindow, public IWorkEvent {
public:
    CCol6CheckBat(CProcess* parent);
    // CProcess overrides
    void Init();
    void Term();
    void Move();
    void Draw();
    virtual ~CCol6CheckBat();

    /* 0x70 */ u8 mFlag70;        // byte flag, init 0
};

// Singleton instance pointer (lbl_eu_80664230 in retail).
extern CCol6CheckBat* gCol6CheckBat;
