// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mps/mps_dec
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803bdf54
void MPSDEC_Init() {}
// LLM-HARNESS-END: us-803bdf54

// LLM-HARNESS-BEGIN: us-803bdf58
void MPSDEC_Finish() {}
// LLM-HARNESS-END: us-803bdf58

// LLM-HARNESS-BEGIN: us-803bdf5c
void MPS_SetSystemFn() {}
// LLM-HARNESS-END: us-803bdf5c

// LLM-HARNESS-BEGIN: us-803bdfb0
void MPS_SetPsMapFn() {}
// LLM-HARNESS-END: us-803bdfb0

// LLM-HARNESS-BEGIN: us-803be004
typedef struct MPS_WORK MPS_WORK;
struct MPS_WORK {
    unsigned char pad[0xf4];
    void *pes_fn;
    void *pes_obj;
};

extern int MPSLIB_CheckHn(MPS_WORK *hn);

void MPS_SetPesFn(MPS_WORK *hn, void *pes_fn, void *pes_obj) {
    if (MPSLIB_CheckHn(hn) == 0) {
        hn->pes_fn = pes_fn;
        hn->pes_obj = pes_obj;
    }
}
// LLM-HARNESS-END: us-803be004

// LLM-HARNESS-BEGIN: us-803be058
void MPS_DecHd() {}
// LLM-HARNESS-END: us-803be058

// LLM-HARNESS-BEGIN: us-803be0dc
void MPSDEC_DecHdMpeg1() {}
// LLM-HARNESS-END: us-803be0dc

// LLM-HARNESS-BEGIN: us-803be1e8
void mpsdec_DecOneHd() {}
// LLM-HARNESS-END: us-803be1e8

// LLM-HARNESS-BEGIN: us-803be2c8
void mpsdec_DecPackHd() {}
// LLM-HARNESS-END: us-803be2c8

// LLM-HARNESS-BEGIN: us-803be53c
void mpsdec_DecSysHd() {}
// LLM-HARNESS-END: us-803be53c

// LLM-HARNESS-BEGIN: us-803bea30
void mpsdec_DecPketHd() {}
// LLM-HARNESS-END: us-803bea30
