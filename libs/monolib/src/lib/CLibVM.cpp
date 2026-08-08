#include "monolib/lib/CLibVM.hpp"

CLibVM::CLibVM(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, 0) {}

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

bool CLibVM::isInitialized() { return false; }

void CLibVM::setCallbacks(void (*callback1)(), void (*callback2)()) {}

bool CLibVM::wkStandbyLogin() { return true; }

bool CLibVM::wkStandbyLogout() { return true; }
