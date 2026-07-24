#include "monolib/device.hpp"

CDeviceFileJob::CDeviceFileJob(const char* pName, CWorkThread* pParent):
CWorkThread(pName, pParent, 0),
mHandle(nullptr),
unk1C8(0),
unk208(0),
unk20C(0),
unk210(0){
    mType = THREAD_CDEVICEFILEJOB;
}

// LLM-HARNESS-BEGIN: us-80454bcc
extern "C" int cancel__14CDeviceFileJobFP25CDeviceFileJob_UnkStruct1(void* self) { return 0; }
// LLM-HARNESS-END: us-80454bcc

bool CDeviceFileJob::cancel(const char* pFilename){
    return false;
}
// LLM-HARNESS-BEGIN: us-80454bd4
extern "C" int CDeviceFileJob_UnkVirtualFunc1__14CDeviceFileJobFv() { return 0; }
// LLM-HARNESS-END: us-80454bd4
