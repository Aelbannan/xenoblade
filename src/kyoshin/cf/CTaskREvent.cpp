// Auto-scaffolded catalog TU for kyoshin/cf/CTaskREvent
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include "kyoshin/cf/CTaskREvent.hpp"

extern "C" void __dt__Q22cf11CTaskREventFv(void*, int);
extern "C" void cbRenderBefore__Q22cf11CTaskREventFv(void*);

namespace cf {}
using namespace cf;

// --- CTTask<cf::CTaskREvent> out-of-line specializations ---
// The canonical declared-only template emits no bodies; these explicit
// specializations produce the retail standalone Move/Draw/dtor symbols.
template<>
void CTTask<cf::CTaskREvent>::Move() {
    if (mMoveFunc) {
        (static_cast<cf::CTaskREvent*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<cf::CTaskREvent>::Draw() {
    if (mDrawFunc) {
        (static_cast<cf::CTaskREvent*>(this)->*mDrawFunc)();
    }
}

template<>
CTTask<cf::CTaskREvent>::~CTTask() {}

class CEventFile;

void __ct__cf_CTaskREvent(){}

cf::CTaskREvent::~CTaskREvent() {}

void func_80164410(){}

void func_80164478(){}

// retail: lwz r3, lbl_eu_80662380; blr
extern void* lbl_eu_80662380;
extern void* lbl_eu_80664240;
extern "C" u32 func_801644AC() { return (u32)lbl_eu_80662380; }

extern "C" u32 func_801644B4() { return (u32)lbl_eu_80664240; }
void func_801644BC__FUl(){}

void func_801644D8(void* self){}

void func_8016455C(){}

void func_8016462C(){}

void func_80164724(){}

void func_80164838(){}

void func_80164910__Fv(){}

void func_80164954(){}

void func_80164A50(){}

void func_80164C28(){}

void func_80164C48(){}

void func_80164CFC(){}

void func_80164DB8(){}

void cf::CTaskREvent::Init() {}

void func_80164ED0(){}

void func_80164F6C(){}

void func_80164FB4(){}

int func_80164FE8(void) {
    int* ptr = (int*)lbl_eu_80664240;
    if (!ptr) return 0;
    int val = *(int*)((char*)ptr + 0x1e0);
    return (unsigned)((-1 - val) | (val + 1)) >> 31;
}

void func_80165014(){}

void func_80165038(void){}

void cf::CTaskREvent::Term() {}

void cf::CTaskREvent::Move() {}

void cf::CTaskREvent::Draw() {}

void func_80165DF4(){}

void func_80166050(){}

void func_80166150(){}

void cf::CTaskREvent::cbRenderBefore() {}

void func_801662E8(void* self){}

void func_801663A8(void* self){}

void func_801665A4(){}

void viAfterDrawDone__11CDeviceVICbFv() {}

void viBeforeDrawDone__11CDeviceVICbFv() {}

void func_801666C4(void* self) { ((void(*)(void*))func_801662E8)((char*)self - 0x54); }

void func_801666CC(void* self) { ((void(*)(void*))__dt__Q22cf11CTaskREventFv)((char*)self - 0x54); }

void func_801666D4(void* self) { ((void(*)(void*))func_801644D8)((char*)self - 0x58); }

void func_801666DC(void* self) { ((void(*)(void*))__dt__Q22cf11CTaskREventFv)((char*)self - 0x58); }

void func_801666E4(void* self) { ((void(*)(void*))cbRenderBefore__Q22cf11CTaskREventFv)((char*)self - 0x5c); }

extern "C" void func_801666EC(u8* self) { ((void(*)(void*))__dt__Q22cf11CTaskREventFv)((char*)self - 0x5c); }

void cf::CTaskREvent::OnFileEvent() { func_801663A8(this); }

extern "C" void func_801666FC(u8* self) { ((void(*)(void*))__dt__Q22cf11CTaskREventFv)((char*)self - 0x60); }

// CTTask<cf::CTaskREvent> specializations provided by header
