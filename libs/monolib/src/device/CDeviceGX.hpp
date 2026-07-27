#pragma once

#include <types.h>

class CDeviceGX {
public:
    virtual ~CDeviceGX();
    void @456@viBeginFrame();
    void @456@viAfterDrawDone();

    // TODO: add fields
    void getInstance();
    void isInitialized();
    void setDevicesInitializedFlag(bool state);
    void devicesInitialized();
    void updateVerticalFilter(EVerticalFilter filter);
    void drawFrame();
    void copyEfb(void* pDestFrameBuffer);
    void onRenderWork();
    void wkStandbyLogin();
    void wkStandbyLogout();
    void drawSyncCallback(u16 token);
    void setValues(GXPixelFmt format, u32 heapSize);
};

