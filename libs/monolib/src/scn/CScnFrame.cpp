// Decompiled for monolib/src/scn/CScnFrame

#include <types.h>
#include "libs/monolib/src/scn/CScnFrame.hpp"

extern "C" {
    extern u32 getRetraceCount__9CDeviceVIFv();

}

CScnFrame::CScnFrame(u32 param) {
    this->mParam = param;
    this->mElapsed = 1.0f;
    this->mStep = 1.0f;
    this->mFlag = 0;
}

CScnFrame::~CScnFrame() {
}

void CScnFrame::update() {
    if (mFlag) {
        u32 tick = getRetraceCount__9CDeviceVIFv();
        f32 factor = 0.5f * (f32)tick;
        mElapsed = mStep * factor;
    } else {
        mElapsed = mStep;
    }
}
