// Auto-scaffolded catalog TU for kyoshin/cf/CTaskREvtSequence
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__cf_CTaskREvtSequence() {}

extern "C" void __dt__801682AC() {}

extern "C" void __dt__Q22cf17CTaskREvtSequenceFv(void* self) {}

extern "C" unsigned long func_801683FC() {
    extern unsigned long lbl_eu_80664268;
    unsigned long* ptr = (unsigned long*)lbl_eu_80664268;
    return (ptr[0x5c/4] >> 17) & 1;
}

extern "C" unsigned long func_8016840C() {
    extern unsigned long lbl_eu_80664268;
    unsigned long* ptr = (unsigned long*)lbl_eu_80664268;
    return (ptr[0x5c/4] >> 19) & 1;
}

extern "C" void func_8016841C() {}

extern "C" void func_8016846C() {}

extern "C" bool func_8016847C() { return false; }

extern "C" void func_80168484() {}

extern "C" void func_801684F4() {}

extern "C" void func_80168514() {}

extern "C" void func_80168610() {}

extern "C" void func_801686B0() {}

extern "C" void func_8016872C() {}

extern "C" void func_80168800() {}

extern "C" void func_80168F38() {}

extern "C" int func_80169048(void* self) { return 1; }

extern "C" void func_80169050() {}

extern "C" void func_8016925C() {}

extern "C" int func_801696C4(void* self) { return 0; }

extern "C" void func_801696CC() {}

extern "C" void func_80169A34() {}

extern "C" void func_80169A38() {}

extern "C" void func_80169CD0() {}

extern "C" void func_80169DD0() {}

extern "C" void func_80169F24() {}

extern "C" void func_80169F28() {}

extern "C" void func_8016A24C() {}

extern "C" void func_8016A27C() {}

extern "C" u32 func_8016A354(void* self) { return *(u32*)((u8*)self + 0x20); }

extern "C" void func_8016A35C() {}

extern "C" void func_8016A378() {}

extern "C" void func_8016A3A8() {}

extern "C" void func_8016A3C4() {}

extern "C" void Init__Q22cf17CTaskREvtSequenceFv() {}

extern "C" void Term__Q22cf17CTaskREvtSequenceFv() {}

extern "C" void func_8016A480() {}

extern "C" void Draw__Q22cf17CTaskREvtSequenceFv() {}

extern "C" void func_8016ABA8(void* self) {}

extern "C" void func_8016AD44() {}

extern "C" void func_8016ADF8() {}

extern "C" void func_8016AED4() {}

extern "C" void func_8016AF4C() {}

extern "C" void func_8016B164() {}

extern "C" void func_8016B384() {}

extern "C" void func_8016B5A4() {}

extern "C" void func_8016B788() {}

extern "C" void func_8016B860() {}

extern "C" void func_8016BB38() {}

extern "C" void func_8016BC1C() {}

extern "C" void func_8016BDA8() {}

extern "C" void func_8016C118() {}

extern "C" void cbRenderBefore__Q22cf17CTaskREvtSequenceFv(void* self) {}

extern "C" void func_8016C2C8() {}

extern "C" void func_8016C2E4() {}

extern "C" void func_8016C300() {}

extern "C" void func_8016C3DC() {}

extern "C" void func_8016C410() {}

extern "C" void func_8016C450() {}

extern "C" void func_8016C6EC() {}

extern "C" void func_8016C720() {}

extern "C" void func_eu_8016DA48() {}

extern "C" void OnFileEvent__17CTaskREvtSequenceFP10CEventFile(void* self) { ((void(*)(void*))func_8016ABA8)((char*)self - 0x54); }

extern "C" void func_8016C7D8(void* self) { ((void(*)(void*))__dt__Q22cf17CTaskREvtSequenceFv)((char*)self - 0x54); }

extern "C" void func_8016C7E0(void* self) { ((void(*)(void*))cbRenderBefore__Q22cf17CTaskREvtSequenceFv)((char*)self - 0x58); }

extern "C" void func_8016C7E8(void* self) { ((void(*)(void*))__dt__Q22cf17CTaskREvtSequenceFv)((char*)self - 0x58); }

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

namespace cf { class CTaskREvtSequence; }
template<> CTTask<cf::CTaskREvtSequence>::~CTTask() {}
template<> void CTTask<cf::CTaskREvtSequence>::Move() {}
template<> void CTTask<cf::CTaskREvtSequence>::Draw() {}
