#include "monolib/lib/CLibVM.hpp"

extern "C" void __ct__6CLibVMFPCcP11CWorkThread() {}

extern "C" void* lbl_eu_80665720;
extern "C" void __dt__11CWorkThreadFv(void*, int);
extern "C" void __dl__FPv(void*);

extern "C" CLibVM* __dt__6CLibVMFv(CLibVM* self, int deleting) {
    if (self != 0) {
        lbl_eu_80665720 = 0;
        __dt__11CWorkThreadFv(self, 0);
        if (deleting > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CLibVM* CLibVM::spInstance;

CLibVM* CLibVM::getInstance() {
    return spInstance;
}

extern "C" void isInitialized__6CLibVMFv() {}

extern "C" void setCallbacks__6CLibVMFPFv_vPFv_v(void* self, u32 a, u32 b) {}

extern "C" void wkStandbyLogin__6CLibVMFv() {}

extern "C" void wkStandbyLogout__6CLibVMFv() {}
