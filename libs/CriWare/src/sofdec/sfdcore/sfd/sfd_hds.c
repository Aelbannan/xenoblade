// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_hds
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803c28fc
void SFHDS_Init(void) {}
// LLM-HARNESS-END: us-803c28fc

// LLM-HARNESS-BEGIN: us-803c2900
void SFHDS_InitFhd(void* self) {
    *(u32*)((u8*)self) = 0;
    *(u32*)((u8*)self + 4) = 0;
    *(u32*)((u8*)self + 8) = 0;
    *(u32*)((u8*)self + 0xc) = 0;
    *(u32*)((u8*)self + 0x90) = 0;
}
// LLM-HARNESS-END: us-803c2900

// LLM-HARNESS-BEGIN: us-803c291c
void SFHDS_FinishFhd(void* self) {
    *(u32*)((u8*)self + 0x00) = 0;
    *(u32*)((u8*)self + 0x0c) = 0;
    *(u32*)((u8*)self + 0x90) = 0;
}
// LLM-HARNESS-END: us-803c291c

// LLM-HARNESS-BEGIN: us-803c2930
void SFHDS_SetHdr() {}
// LLM-HARNESS-END: us-803c2930

// LLM-HARNESS-BEGIN: us-803c2b60
void SFHDS_ReprocessHdr() {}
// LLM-HARNESS-END: us-803c2b60

// LLM-HARNESS-BEGIN: us-803c2c4c
void sfhds_DoProcessHdr() {}
// LLM-HARNESS-END: us-803c2c4c

// LLM-HARNESS-BEGIN: us-803c3070
void sfhds_AnlyVideo() {}
// LLM-HARNESS-END: us-803c3070

// LLM-HARNESS-BEGIN: us-803c32a4
typedef struct {
    unsigned char pad0[0x88];
    int field_0x88;
    int field_0x8c;
    int field_0x90;
} SfdHdsStruct;

int SFHDS_GetMuxVerNum(SfdHdsStruct* this) {
    if (this->field_0x88 == 0) {
        return 0;
    }
    return this->field_0x8c * 100 + this->field_0x90;
}
// LLM-HARNESS-END: us-803c32a4

// LLM-HARNESS-BEGIN: us-803c32cc
int SFHDS_GetColType(void* obj) {
    struct SubStruct {
        unsigned char pad[0x14];
        int field_14;
        int field_18;
    };
    struct MainStruct {
        unsigned char pad0[0x88];
        int flag;
        unsigned char pad1[0x58];
        struct SubStruct sub;
    };
    struct MainStruct* p = (struct MainStruct*)obj;
    if (p->flag == 0) return -1;
    struct SubStruct* s = &p->sub;
    if (s->field_14 == 0) return -1;
    return s->field_18;
}
// LLM-HARNESS-END: us-803c32cc

// LLM-HARNESS-BEGIN: us-803c3300
extern char lbl_eu_80606E38[];
void* fn_803C3300(void) {
    return lbl_eu_80606E38 + 0x21c;
}
// LLM-HARNESS-END: us-803c3300
