#include <revolution/OS.h>

extern "C" {

void MWInitializeCriticalSection(unsigned int* section){
}

void MWEnterCriticalSection(unsigned int* section){
    *section = OSDisableInterrupts();
}

void MWExitCriticalSection(unsigned int* section) {
    OSRestoreInterrupts(*section);
}

// not present in the retail binary; kept commented out for reference
//void MWTerminateCriticalSection(unsigned int* section){
//}

}
