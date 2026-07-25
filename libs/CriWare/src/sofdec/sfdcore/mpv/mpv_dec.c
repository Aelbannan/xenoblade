// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_dec
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803a7ea0
void MPVDEC_CheckVersion() {}
// LLM-HARNESS-END: us-803a7ea0

// LLM-HARNESS-BEGIN: us-803a7f18
void MPVDEC_DecIpicMb() {}
// LLM-HARNESS-END: us-803a7f18

// LLM-HARNESS-BEGIN: us-803a8300
void MPVDEC_DecPpicMb() {}
// LLM-HARNESS-END: us-803a8300

// LLM-HARNESS-BEGIN: us-803a88c0
void MPVDEC_ResetMv(void* self) {
    *(u32*)((u8*)self + 0x10) = 0;
    *(u32*)((u8*)self + 0x14) = 0;
    *(u32*)((u8*)self + 0x18) = 0;
    *(u32*)((u8*)self + 0x1c) = 0;
}
// LLM-HARNESS-END: us-803a88c0

// LLM-HARNESS-BEGIN: us-803a88d8
void MPVDEC_ResetDc(void* self) {
    *(u32*)((u8*)self + 0xcf4) = 0x400;
    *(u32*)((u8*)self + 0xcf8) = 0x400;
    *(u32*)((u8*)self + 0xcf0) = 0x400;
}
// LLM-HARNESS-END: us-803a88d8

// LLM-HARNESS-BEGIN: us-803a88ec
void mpvdec_MotionSub() {}
// LLM-HARNESS-END: us-803a88ec

// LLM-HARNESS-BEGIN: us-803a8a78
void MPVDEC_DecBpicMb() {}
// LLM-HARNESS-END: us-803a8a78
