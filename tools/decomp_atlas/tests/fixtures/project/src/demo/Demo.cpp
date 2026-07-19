#include "Demo.hpp"

namespace {

int gDummy;

}  // namespace

// LLM-HARNESS-BEGIN: demo-leaf
void Demo::leaf() {
    gDummy = 1;
}
// LLM-HARNESS-END: demo-leaf

// LLM-HARNESS-BEGIN: demo-accepted
void Demo::accepted() {
    gDummy = 2;
}
// LLM-HARNESS-END: demo-accepted

// LLM-HARNESS-BEGIN: demo-callees-ready
void Demo::ready() {
    accepted();
}
// LLM-HARNESS-END: demo-callees-ready

// LLM-HARNESS-BEGIN: demo-blocked-unresolved
void Demo::blocked() {
    accepted();
}
// LLM-HARNESS-END: demo-blocked-unresolved

// LLM-HARNESS-BEGIN: demo-blocked-indirect
void Demo::indirect() {
    accepted();
}
// LLM-HARNESS-END: demo-blocked-indirect

// LLM-HARNESS-BEGIN: demo-ctor
Demo::Demo() : mValue(0) {}
// LLM-HARNESS-END: demo-ctor

// LLM-HARNESS-BEGIN: demo-overload
void Demo::draw(int mode) {
    leaf();
    gDummy = mode;
}
// LLM-HARNESS-END: demo-overload

void Demo::draw() {
    draw(0);
}
