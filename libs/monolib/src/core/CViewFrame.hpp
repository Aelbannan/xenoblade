#pragma once

#include <types.h>
#include "monolib/core/CView.hpp"
#include "monolib/core/CDrawGX.hpp"
#include "monolib/core/CViewRectData.hpp"

class CViewFrame {
public:
    CViewFrame();
    void CView_UnkVirtualFunc1();
    void func_804406D8();
    void func_804409D0();
    void func_80440D78();
    void func_80441290();
    void func_80441310();
    void detachRenderWork();
    void CView_UnkVirtualFunc9();
    void CView_UnkVirtualFunc8();
    void func_80441EF0();

    // TODO: add fields
    void func_8043FC60();
    void getFrame2ViewOffset();
    void func_8043FD10();
};

