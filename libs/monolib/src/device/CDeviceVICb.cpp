#include "monolib/device.hpp"

CDeviceVICb::CDeviceVICb(){
    *(void**)this = (void*)lbl_eu_8056BF68;
    CDeviceVI::entryCb(this);
}

CDeviceVICb::~CDeviceVICb(){
    *(void**)this = (void*)lbl_eu_8056BF68;
    CDeviceVI::removeCb(this);
}
