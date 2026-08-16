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

// When the +0x20 sub-object is visible (func_80137510 with the 1.0 constant),
// reset the selection state bytes (m2C/m2E/m2F) for a fresh pass.
extern "C" u32 func_80137510(void*, float);
extern "C" void func_801EB530(void* self) {
    extern const f32 lbl_eu_80668088;
    if (func_80137510(*(void**)((u8*)self + 0x20), lbl_eu_80668088)) {
        *((u8*)self + 0x2F) = 0;
        *((u8*)self + 0x2E) = 1;
        *((u8*)self + 0x2C) = 0;
    }
}

void func_801EB580(){}

void func_801EB644(){}

void CNumSelect::OnFileEvent() {}

// Out-of-line dtor (retail symbol __dt__10CNumSelectFv); the inline header
// body would not emit the symbol.
#pragma optimize_for_size on  // -O4,s keeps the retail stmw r30 frame
CNumSelect::~CNumSelect() {}
#pragma optimize_for_size off

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
