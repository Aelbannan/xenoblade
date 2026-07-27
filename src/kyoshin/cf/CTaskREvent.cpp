// Auto-scaffolded catalog TU for kyoshin/cf/CTaskREvent
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void __ct__cf_CTaskREvent(){}

void cf::CTaskREvent::~CTaskREvent() {}

void func_80164410(){}

void func_80164478(){}

bool func_801644AC() { return false; }

bool func_801644B4() { return false; }

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

void* lbl_eu_80664240;

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

extern "C" void func_801666EC(void* self) { ((void(*)(void*))__dt__Q22cf11CTaskREventFv)((char*)self - 0x5c); }

extern "C" void OnFileEvent__11CTaskREventFP10CEventFile(void* self) { ((void(*)(void*))func_801663A8)((char*)self - 0x60); }

extern "C" void func_801666FC(void* self) { ((void(*)(void*))__dt__Q22cf11CTaskREventFv)((char*)self - 0x60); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Local CTTask (out-of-line Move/Draw/dtor) for harness stubs.
// Do not include monolib/work/CTTask.hpp here — its inline methods collide.
template <typename T>
class CTTask {
public:
    CTTask();
    virtual ~CTTask();
    virtual void Move();
    virtual void Draw();
};

namespace cf { class CTaskREvent; }
template<> CTTask<cf::CTaskREvent>::~CTTask() {}
template<> void CTTask<cf::CTaskREvent>::Move() {}
template<> void CTTask<cf::CTaskREvent>::Draw() {}
