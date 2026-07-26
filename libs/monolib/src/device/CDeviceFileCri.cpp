#include "monolib/device.hpp"

extern const wchar_t* lbl_80665FF0;
extern const wchar_t* lbl_80665FF4;
extern const wchar_t* lbl_80665FF8;

void CDeviceFileCri::func_80450B14(const wchar_t* pData){
    lbl_80665FF0 = pData;
}

void CDeviceFileCri::func_80450B1C(const wchar_t* pData){
    lbl_80665FF4 = pData;
}

void CDeviceFileCri::func_80450B24(const wchar_t* pData){
    lbl_80665FF8 = pData;
}

extern "C" void getInstance__14CDeviceFileCriFv() {}
extern "C" void getInstance__14CDeviceFileCriFv() {}
extern "C" void getInstance__14CDeviceFileCriFv() {}
extern "C" void getInstance__14CDeviceFileCriFv() {}
extern "C" void sinit_80450B2C() {}
extern "C" {
void func_80450AB8__14CDeviceFileCriFUl(void* this_ptr, unsigned long arg);
}

extern "C" void func_80450B44(void* this_ptr, unsigned long arg) {
    func_80450AB8__14CDeviceFileCriFUl((char*)this_ptr - 0x1c4, arg);
}
extern "C" void __dt__14CDeviceFileCriFv(void*);

extern "C" void func_80450B4C(void* self) {
    __dt__14CDeviceFileCriFv((char*)self - 0x1c4);
}

extern "C" void getInstance__14CDeviceFileCriFv() {}
