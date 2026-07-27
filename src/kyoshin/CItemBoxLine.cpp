// Auto-scaffolded catalog TU for kyoshin/CItemBoxLine
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

u8 func_801ED800(void* self) { return static_cast<CItemBoxLine*>(self)->unk59; }










void func_801EC438(){}

void func_801EC808(){}

void func_801EC8B4(){}

void func_801EC8D8(){}

void func_801EC9E0(){}

void __ct__CIBLTabCur(){}

void __dt__801ECBC0(){}

void func_801ECC10(){}

void __ct__CItemBoxLine(){}

void CItemBoxLine::~CItemBoxLine() {}

void func_801ED31C(){}

void func_801ED3E8(){}

void func_801ED4FC(){}

void func_801ED618(){}

void func_801ED774(){}


void func_801ED808(){}

void func_801ED864(){}

void func_801ED97C(){}

void func_801EDA08(){}

void CItemBoxLine::func_801EDA4C(unsigned char val) {
    unsigned char n = reinterpret_cast<unsigned char*>(this)[0x63];
    if (n >= 9) {
        return;
    }
    reinterpret_cast<unsigned char*>(this)[0x5a + n] = val;
    reinterpret_cast<unsigned char*>(this)[0x63] = n + 1;
}

void func_801EDA6C(){}

void func_801EDB80(){}

void func_801EDC94(){}

void func_801EDF40(){}

void func_801EE228(){}

void func_801EE448(){}

void func_801EE684(){}

void func_801EE788(){}

u8 func_801EECC0(void* self) { return static_cast<CItemBoxLine*>(self)->unk39E; }

void func_801EC3B0(void*, unsigned int);
void CItemBoxLine::func_801EECC8() {
    func_801EC3B0(&unk3A4, (unsigned char)(unk38C + unk38E));
}

void func_801D2E4C(void* self);
void func_801EECE0(void* self) { ((void(*)(void*))func_801D2E4C)((char*)self + 0xb8); }

void func_801EECE8(){}

void func_801EED6C(){}

void func_801EEDF8(){}

void func_801EF034(){}

void func_801EF050(){}

void func_801EF0EC(){}

void func_801EF1E4(){}

void func_801EF260(){}

void func_801EF2FC(){}

void func_801EF378(){}

void func_801EF3E8(){}

void func_801EF45C(){}

void func_801EF518(){}

void func_801EF734(){}

void func_801EF844(){}

void func_801EF954(){}

void func_801EFB24(){}

void func_801EFDF4(){}

void func_801EFE6C(){}

void func_801EFFC4(){}

void func_801F0030(){}

void func_801F0488(){}

void func_801F061C(){}

void func_801F071C(){}

void func_801F08B4(){}

void func_801F0A58(){}

void func_801F107C(){}

void func_801F183C(){}

void func_801F1E64(){}

void func_801F20F0(){}

void func_801F2298(){}

void func_801F2434(){}

void func_801F2880(){}

void CItemBoxLine::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_801F32EC(){}

extern "C" void func_801EBB88(void* self, u16 r4, u32 r5, u8 r6, u8 r7) {
    *(u16*)self = r4;
    *(u32*)((u8*)self + 4) = r5;
    *((u8*)self + 8) = r6;
    *((u8*)self + 9) = r7;
}
