// CVS_THREAD_PARTY_GAGE - voice thread for party gauge change events.
// Virtual method overrides and thread management for party gauge voice processing.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_PARTY_GAGE.hpp"
#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-802aa8a8
extern "C" void __ct__CVS_THREAD_PARTY_GAGE() {}
// LLM-HARNESS-END: us-802aa8a8

// LLM-HARNESS-BEGIN: us-802aaa08
extern "C" void func_802A82D4() {}
// LLM-HARNESS-END: us-802aaa08

// LLM-HARNESS-BEGIN: us-802aab00
extern "C" void func_802A83CC() {}
// LLM-HARNESS-END: us-802aab00

// LLM-HARNESS-BEGIN: us-802aac94
extern "C" void func_802A8560() {}
// LLM-HARNESS-END: us-802aac94

// LLM-HARNESS-BEGIN: us-802aacdc
extern "C" void func_802A85A8() {}
// LLM-HARNESS-END: us-802aacdc

// LLM-HARNESS-BEGIN: us-802aad54
// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank2 slot in vtable; PARTY_GAGE subclass returns 0x8C (140).
extern "C" int func_802A8620(CVS_THREAD_PARTY_GAGE* self) {
    return CVS_THREAD_PARTY_GAGE::BUFFER_SIZE;
}
// LLM-HARNESS-END: us-802aad54

// LLM-HARNESS-BEGIN: us-802aad5c
extern "C" void func_802A8628() {}
// LLM-HARNESS-END: us-802aad5c
