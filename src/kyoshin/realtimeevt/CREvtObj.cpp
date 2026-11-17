// TU: kyoshin/realtimeevt/CREvtObj
// Class: cf::CREvtObj (namespace cf) - base of the realtime-event objects
// (camera, effect, light, model, movie). Size 0x14.
//
// Objects live in the CREvtMem arena: the class has its own operator
// new/delete, so the deleting destructor releases through freeEvtMemPtr.

#include "kyoshin/realtimeevt/CREvtObj.hpp"

namespace cf {

CREvtObj::CREvtObj(int type) : mType(type), mCallback(nullptr) {}

CREvtObj::~CREvtObj() {}

// Runs the stored callback when one is set.
void CREvtObj::vfunc_1C() {
    if (mCallback) {
        (this->*mCallback)();
    }
}

void* CREvtObj::operator new(size_t size) {
    return allocEvtMemAligned(size, 4, 1);
}

void CREvtObj::operator delete(void* p) {
    freeEvtMemPtr(static_cast<u8*>(p));
}

int CREvtObj::vfunc_0C() { return 0; }

} // namespace cf
