// Auto-scaffolded catalog TU for kyoshin/CQuestWindow
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" u32 func_80122448(void) {
    extern u32 lbl_eu_80663FD0;
    return lbl_eu_80663FD0;
}

extern "C" int lbl_eu_80663D1C;

extern "C" int func_80042864() {
    return lbl_eu_80663D1C != 0;
}

extern u32 lbl_eu_80663F24;
void func_80122460(){ lbl_eu_80663F24 = 0; }

void func_80122654(){}

void func_801226C8(){}

void func_8012278C(void* self){}

void CQuestWindow::cbRenderBefore() {}

void func_80122B2C(){}

void func_80122BB0(){}

void func_80122C08(){}

void func_80122EF8(){}

void func_801231C4(){}

void func_80124270(){}

void func_80124288(void* self, void* src){
    float a = *(float*)src;
    float b = *(float*)((u8*)src + 4);
    *(float*)((u8*)self + 0x4C) = a;
    *(float*)((u8*)self + 0x50) = b;
}

void func_8012429C(){}

void func_801242FC(){}

void func_8012435C(){}

void __dt__12CQuestWindowFv();
extern "C" void OnFileEvent__12CQuestWindowFP10CEventFile(void* self) { ((void(*)(void*))func_8012278C)((char*)self - 0x6c); }

void func_801245D4(void* self) { ((void(*)(void*))__dt__12CQuestWindowFv)((char*)self - 0x6c); }

extern "C" void func_801245DC(void* self) { ((void(*)(void*))cbRenderBefore__12CQuestWindowFv)((char*)self - 0x70); }

extern "C" void func_801245E4(void* self) { ((void(*)(void*))__dt__12CQuestWindowFv)((char*)self - 0x70); }

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

class IUIWindow;
template<> void CTTask<IUIWindow>::Move() {}
template<> void CTTask<IUIWindow>::Draw() {}
