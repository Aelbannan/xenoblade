// Auto-scaffolded catalog TU for kyoshin/CTaskGameEvt
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void OnFileEvent__12CTaskGameEvtFP10CEventFile(void* self) { ((void(*)(void*))func_80295764)((char*)self - 0x54); }


void func_802956A4(void){}

void func_802956A8(void){}




extern "C" void cbRenderBefore__12CTaskGameEvtFv(void* self) {}

extern "C" int func_80295764(void* self) { return 0; }





void func_80295870(void* self) { ((void(*)(void*))__dt__12CTaskGameEvtFv)((char*)self - 0x54); }

extern "C" void func_80295878(void* self) { ((void(*)(void*))cbRenderBefore__12CTaskGameEvtFv)((char*)self - 0x58); }

extern "C" void func_80295880(void* self) { ((void(*)(void*))__dt__12CTaskGameEvtFv)((char*)self - 0x58); }

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

class CTaskGameEvt;
extern "C" void removeRenderCB__4CScnFP10IScnRender(void*, void*);
void CTaskGameEvt::Term() {
    void* r4 = this ? (u8*)this + 0x58 : 0;
    void* scn = *(void**)((u8*)this + 0x5C);
    removeRenderCB__4CScnFP10IScnRender(scn, r4);
}
