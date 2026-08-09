// Auto-scaffolded catalog TU for kyoshin/CNumSelect
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CNumSelect.hpp"
struct CNumSelectFull;
namespace nw4r {
namespace lyt {
class Layout;
}
}

u8 func_801EB018(CNumSelectFull* self) { return self->field_2D; }


u8 func_801EB020(CNumSelectFull* self) { return self->field_2C; }

u8 func_801EB028(CNumSelectFull* self) { return self->field_2E; }


void func_801EB178(){}

void func_801EB218(){}

void func_801EB314(){}

void func_801EB410(){}

void func_801EB49C(){}

void func_801EB530(){}

void func_801EB580(){}

void func_801EB644(){}

void CNumSelect::OnFileEvent() {}

extern u8 lbl_eu_80506C14[];
extern "C" void func_801EB04C(u8* self, u8 r4) {
    nw4r::lyt::Layout* layout = *(nw4r::lyt::Layout**)(self + 0x1C);
    func_80136910__FPQ34nw4r3lyt6LayoutPcUc(layout, (char*)lbl_eu_80506C14 + 0x20, r4);
}

extern "C" void func_801EAE8C() {}
extern "C" void func_801EAED4() {}
extern "C" void func_801EAF7C() {}
extern "C" void func_801EAF9C() {}
extern "C" void func_801EB030() {}
extern "C" void func_801EB064() {}
extern "C" void func_801EB0D4() {}
