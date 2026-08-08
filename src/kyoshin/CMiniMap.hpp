#pragma once

#include <types.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" float lbl_eu_806670A0;
extern "C" float lbl_eu_80661E48;
extern "C" float lbl_eu_806670CC;
extern "C" float lbl_eu_80663FB4;

class CMMClock {
public:
    virtual ~CMMClock();
    void OnFileEvent();

    // TODO: add fields
};

class CMMTex {
public:
    virtual ~CMMTex() {}
    void OnFileEvent();

    // TODO: add fields
};

class CMiniMap {
public:
    CMiniMap();
    virtual ~CMiniMap();
    void OnFileEvent();

    // TODO: add fields
};

class CMenuMiniMap2 {
public:
    virtual ~CMenuMiniMap2();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

