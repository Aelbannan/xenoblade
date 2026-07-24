#include "monolib/lib/CLibVM.hpp"

// LLM-HARNESS-BEGIN: us-804640b0
extern "C" void __ct__6CLibVMFPCcP11CWorkThread() {}
// LLM-HARNESS-END: us-804640b0

// LLM-HARNESS-BEGIN: us-804640fc
extern "C" void __dt__6CLibVMFv() {}
// LLM-HARNESS-END: us-804640fc

CLibVM* CLibVM::spInstance;

// LLM-HARNESS-BEGIN: us-8046415c
CLibVM* CLibVM::getInstance() {
    return spInstance;
}
// LLM-HARNESS-END: us-8046415c

// LLM-HARNESS-BEGIN: us-80464164
extern "C" void isInitialized__6CLibVMFv() {}
// LLM-HARNESS-END: us-80464164

// LLM-HARNESS-BEGIN: us-8046420c
extern "C" void setCallbacks__6CLibVMFPFv_vPFv_v(void* self, u32 a, u32 b) {}
// LLM-HARNESS-END: us-8046420c

// LLM-HARNESS-BEGIN: us-80464218
extern "C" void wkStandbyLogin__6CLibVMFv() {}
// LLM-HARNESS-END: us-80464218

// LLM-HARNESS-BEGIN: us-80464270
extern "C" void wkStandbyLogout__6CLibVMFv() {}
// LLM-HARNESS-END: us-80464270
