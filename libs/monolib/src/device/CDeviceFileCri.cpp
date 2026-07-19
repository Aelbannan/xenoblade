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
CDeviceFileCri* CDeviceFileCri::getInstance() { return 0; }
// LLM-HARNESS-END: us-80452610
// LLM-HARNESS-BEGIN: us-80452618
void CDeviceFileCri::func_8044F964() {}
// LLM-HARNESS-END: us-80452618
// LLM-HARNESS-BEGIN: us-8045291c
void CDeviceFileCri::func_8044FC38() {}
// LLM-HARNESS-END: us-8045291c
// LLM-HARNESS-BEGIN: us-80453840
extern "C" void sinit_80450B2C() {}
// LLM-HARNESS-END: us-80453840
// LLM-HARNESS-BEGIN: us-80453858
extern "C" bool func_80450B44() { return false; }
// LLM-HARNESS-END: us-80453858
// LLM-HARNESS-BEGIN: us-80453860
extern "C" bool func_80450B4C() { return false; }
// LLM-HARNESS-END: us-80453860

// LLM-HARNESS-BEGIN: us-8045233c
CDeviceFileCri::~CDeviceFileCri() {}
// LLM-HARNESS-END: us-8045233c
