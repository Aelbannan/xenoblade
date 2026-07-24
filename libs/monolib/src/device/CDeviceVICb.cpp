#include "monolib/device.hpp"

CDeviceVICb::CDeviceVICb(){
    CDeviceVI::entryCb(this);
}

CDeviceVICb::~CDeviceVICb(){
    CDeviceVI::removeCb(this);
}

// Only the functions that retail actually emits
void CDeviceVICb::viBeginFrame(){}
