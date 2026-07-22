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

// LLM-HARNESS-BEGIN: us-80452260
extern "C" void __ct__CDeviceFileCri() {}
// LLM-HARNESS-END: us-80452260
// LLM-HARNESS-BEGIN: us-80452610
extern "C" void getInstance__14CDeviceFileCriFv() {}
// LLM-HARNESS-END: us-80452610
// LLM-HARNESS-BEGIN: us-80452618
extern "C" void func_8044F964__14CDeviceFileCriFv() {}
// LLM-HARNESS-END: us-80452618
// LLM-HARNESS-BEGIN: us-8045291c
extern "C" void func_8044FC38__14CDeviceFileCriFv() {}
// LLM-HARNESS-END: us-8045291c
// LLM-HARNESS-BEGIN: us-80453840
extern "C" void sinit_80450B2C() {}
// LLM-HARNESS-END: us-80453840
// LLM-HARNESS-BEGIN: us-80453858
extern "C" {
void func_80450AB8__14CDeviceFileCriFUl(void* this_ptr, unsigned long arg);
}

extern "C" void func_80450B44(void* this_ptr, unsigned long arg) {
    func_80450AB8__14CDeviceFileCriFUl((char*)this_ptr - 0x1c4, arg);
}
// LLM-HARNESS-END: us-80453858
// LLM-HARNESS-BEGIN: us-80453860
extern "C" void __dt__14CDeviceFileCriFv(void*);

extern "C" void func_80450B4C(void* self) {
    __dt__14CDeviceFileCriFv((char*)self - 0x1c4);
}
// LLM-HARNESS-END: us-80453860

// LLM-HARNESS-BEGIN: us-8045233c
extern "C" void __dt__14CDeviceFileCriFv() {}
// LLM-HARNESS-END: us-8045233c
