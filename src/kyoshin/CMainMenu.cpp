// Auto-scaffolded catalog TU for kyoshin/CMainMenu
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-800ff518
extern "C" void __dt__800FEA30() {}
// LLM-HARNESS-END: us-800ff518

// LLM-HARNESS-BEGIN: us-800ff570
extern "C" void func_800FEA88() {}
// LLM-HARNESS-END: us-800ff570

// LLM-HARNESS-BEGIN: us-800ff5fc
extern "C" void func_800FEB14() {}
// LLM-HARNESS-END: us-800ff5fc

// LLM-HARNESS-BEGIN: us-800ff66c
extern "C" void __ct__CMainMenu() {}
// LLM-HARNESS-END: us-800ff66c

// LLM-HARNESS-BEGIN: us-800ff7f4
extern "C" void __dt__800FED0C() {}
// LLM-HARNESS-END: us-800ff7f4

// LLM-HARNESS-BEGIN: us-800ff850
extern "C" void __dt__9CMainMenuFv(void* self) {}
// LLM-HARNESS-END: us-800ff850

// LLM-HARNESS-BEGIN: us-800ff8e0
extern "C" bool func_800FEDF8() { return false; }
// LLM-HARNESS-END: us-800ff8e0

// LLM-HARNESS-BEGIN: us-800ff8e8
extern "C" void Init__9CMainMenuFv() {}
// LLM-HARNESS-END: us-800ff8e8

// LLM-HARNESS-BEGIN: us-800ff944
extern "C" void Term__9CMainMenuFv() {}
// LLM-HARNESS-END: us-800ff944

// LLM-HARNESS-BEGIN: us-800ffa08
extern "C" void func_800FEF20() {}
// LLM-HARNESS-END: us-800ffa08

// LLM-HARNESS-BEGIN: us-800ffa34
extern "C" void func_800FEF4C() {}
// LLM-HARNESS-END: us-800ffa34

// LLM-HARNESS-BEGIN: us-800ffde8
extern "C" void __ct__800FF300(void* self) {}
// LLM-HARNESS-END: us-800ffde8

// LLM-HARNESS-BEGIN: us-801000ec
extern "C" void cbRenderBefore__9CMainMenuFv(void* self) {}
// LLM-HARNESS-END: us-801000ec

// LLM-HARNESS-BEGIN: us-801001a4
extern "C" void func_800FF6BC() {}
// LLM-HARNESS-END: us-801001a4

// LLM-HARNESS-BEGIN: us-80100220
extern "C" void func_800FF738() {}
// LLM-HARNESS-END: us-80100220

// LLM-HARNESS-BEGIN: us-80100260
extern "C" void func_800FF778__9CMainMenuFv() {}
// LLM-HARNESS-END: us-80100260

// LLM-HARNESS-BEGIN: us-80100398
extern "C" void func_800FF8B0() {}
// LLM-HARNESS-END: us-80100398

// LLM-HARNESS-BEGIN: us-801003fc
extern "C" void func_800FF914() {}
// LLM-HARNESS-END: us-801003fc

// LLM-HARNESS-BEGIN: us-80100408
extern "C" void func_800FF920() {}
// LLM-HARNESS-END: us-80100408

// LLM-HARNESS-BEGIN: us-801018fc
extern "C" void func_80100E14() {}
// LLM-HARNESS-END: us-801018fc

// LLM-HARNESS-BEGIN: us-80101ba0
extern "C" void func_801010B8() {}
// LLM-HARNESS-END: us-80101ba0

// LLM-HARNESS-BEGIN: us-801023dc
extern "C" void func_801018F4() {}
// LLM-HARNESS-END: us-801023dc

// LLM-HARNESS-BEGIN: us-80102570
extern "C" void func_80101A88() {}
// LLM-HARNESS-END: us-80102570

// LLM-HARNESS-BEGIN: us-801026e0
extern "C" void func_80101BF8() {}
// LLM-HARNESS-END: us-801026e0

// LLM-HARNESS-BEGIN: us-80102ae8
extern "C" void OnFileEvent__9CMainMenuFP10CEventFile(void* self) { ((void(*)(void*))__ct__800FF300)((char*)self - 0x58); }
// LLM-HARNESS-END: us-80102ae8

// LLM-HARNESS-BEGIN: us-80102af0
extern "C" void func_80102008(void* self) { ((void(*)(void*))__dt__9CMainMenuFv)((char*)self - 0x58); }
// LLM-HARNESS-END: us-80102af0

// LLM-HARNESS-BEGIN: us-80102af8
extern "C" void func_80102010(void* self) { ((void(*)(void*))cbRenderBefore__9CMainMenuFv)((char*)self - 0x5c); }
// LLM-HARNESS-END: us-80102af8

// LLM-HARNESS-BEGIN: us-80102b00
extern "C" void func_80102018(void* self) { ((void(*)(void*))__dt__9CMainMenuFv)((char*)self - 0x5c); }
// LLM-HARNESS-END: us-80102b00

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

class IUICf;
// LLM-HARNESS-BEGIN: us-800ff79c
template<> CTTask<IUICf>::~CTTask() {}
// LLM-HARNESS-END: us-800ff79c
// LLM-HARNESS-BEGIN: us-80102a58
template<> void CTTask<IUICf>::Move() {}
// LLM-HARNESS-END: us-80102a58
// LLM-HARNESS-BEGIN: us-80102aa0
template<> void CTTask<IUICf>::Draw() {}
// LLM-HARNESS-END: us-80102aa0
