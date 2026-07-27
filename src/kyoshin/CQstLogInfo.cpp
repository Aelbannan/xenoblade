// Auto-scaffolded catalog TU for kyoshin/CQstLogInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

u8 func_802296D0(void* self) { return ((u8*)self)[0x38]; }








u8 func_802296D8(void* self) { return ((u8*)self)[0x39]; }



void func_80229768(void* self, u16 val) { *(u16*)((u8*)self + 0x3A) = val; }


void func_802297BC(){}

void func_80229808(){}

void func_80229854(){}

void func_802298A0(){}

void func_80229900(){}

void func_80229960(){}

void func_80229A0C(){}

void func_80229B54(){}

void func_80229CA0(){}

void func_80229CF0(){}

void func_8022A904(){}

void func_8022AFF8(){}

void CQstLogInfo::OnFileEvent() {}

extern "C" void func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(void*, void*, int, int);
void func_80229600(void* self){
    if (*(u8*)((u8*)self + 0x30) == 0) return;
    func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(*(void**)((u8*)self + 0x20), 0, 0, 1);
}
