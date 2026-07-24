// Auto-scaffolded catalog TU for monolib/src/lib/CLibLayout
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "monolib/lib/CLibLayout.hpp"

// LLM-HARNESS-BEGIN: us-80462f54
extern "C" void __ct__10CLibLayoutFPCcP11CWorkThread() {}
// LLM-HARNESS-END: us-80462f54

// LLM-HARNESS-BEGIN: us-80462fd0
extern "C" void __dt__8045F000() {}
// LLM-HARNESS-END: us-80462fd0

// LLM-HARNESS-BEGIN: us-8046301c
extern "C" void __dt__10CLibLayoutFv() {}
// LLM-HARNESS-END: us-8046301c

// LLM-HARNESS-BEGIN: us-80463090
extern "C" void getAllocHandle__10CLibLayoutFv(void) {}
// LLM-HARNESS-END: us-80463090

// LLM-HARNESS-BEGIN: us-8046309c
// Returns the global CLibLayout singleton instance.
CLibLayout* CLibLayout::getInstance() {
    extern CLibLayout* lbl_eu_80665710; // sbss singleton pointer
    return lbl_eu_80665710;
}
// LLM-HARNESS-END: us-8046309c

// LLM-HARNESS-BEGIN: us-804630a4
extern "C" void isInitialized__10CLibLayoutFv() {}
// LLM-HARNESS-END: us-804630a4

// LLM-HARNESS-BEGIN: us-8046314c
extern "C" void createLayout__10CLibLayoutFv() {}
// LLM-HARNESS-END: us-8046314c

// LLM-HARNESS-BEGIN: us-80463184
extern "C" void createArcResourceAccessor__10CLibLayoutFv() {}
// LLM-HARNESS-END: us-80463184

// LLM-HARNESS-BEGIN: us-804631bc
extern "C" void createPicture__10CLibLayoutFv() {}
// LLM-HARNESS-END: us-804631bc

// LLM-HARNESS-BEGIN: us-80463264
extern "C" void createTextbox__10CLibLayoutFv() {}
// LLM-HARNESS-END: us-80463264

// LLM-HARNESS-BEGIN: us-804632dc
extern "C" void deleteTextboxOrPicture__10CLibLayoutFv() {}
// LLM-HARNESS-END: us-804632dc

// LLM-HARNESS-BEGIN: us-804632ec
// No-op override; CLibLayout performs no per-frame work.
void CLibLayout::wkUpdate() {}
// LLM-HARNESS-END: us-804632ec

// LLM-HARNESS-BEGIN: us-804632f0
extern "C" void wkStandbyLogin__10CLibLayoutFv() {}
// LLM-HARNESS-END: us-804632f0

// LLM-HARNESS-BEGIN: us-804633a4
extern "C" void wkStandbyLogout__10CLibLayoutFv() {}
// LLM-HARNESS-END: us-804633a4

// LLM-HARNESS-BEGIN: us-80463408
extern "C" void func_8045F438__10CLibLayoutFv() {}
// LLM-HARNESS-END: us-80463408

// LLM-HARNESS-BEGIN: us-804634b4
extern "C" void func_8045F4E4__10CLibLayoutFv() {}
// LLM-HARNESS-END: us-804634b4
