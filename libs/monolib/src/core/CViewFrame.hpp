#pragma once

#include <types.h>
#include "monolib/core/CView.hpp"
#include "monolib/core/CDrawGX.hpp"
#include "monolib/core/CViewRectData.hpp"

class CViewFrame {
public:
    CViewFrame();
    void CView_UnkVirtualFunc1();
    void drawSplitBorder();
    void drawFrameBorder();
    void drawBorderEdge();
    void updateViewFrameRect();
    void hitTestFrame();
    void detachRenderWork();
    void CView_UnkVirtualFunc9();
    void CView_UnkVirtualFunc8();
    void getFrame2ViewRect();

    // TODO: add fields
    void setViewFrameMode();
    void getFrame2ViewOffset();
    void func_8043FD10();
};

