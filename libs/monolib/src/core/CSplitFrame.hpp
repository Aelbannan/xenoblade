#pragma once

#include <types.h>
#include "monolib/core/CViewRoot.hpp"
#include "monolib/core/CView.hpp"
#include "monolib/core/CViewFrame.hpp"

class CSplitFrame {
public:
    void apply();
    void getSplitLine();
    void setSplitLine();
    void getScissorRect1();
    void getScissorRect2();

    // TODO: add fields
    void getView1();
    void getView2();
    void isActive();
};

