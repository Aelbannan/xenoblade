// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_hds
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
void SFHDS_Init(void) { SFH_Init(); }


void SFHDS_InitFhd(void* self) {
    *(u32*)((u8*)self) = 0;
    *(u32*)((u8*)self + 4) = 0;
    *(u32*)((u8*)self + 8) = 0;
    *(u32*)((u8*)self + 0xc) = 0;
    *(u32*)((u8*)self + 0x90) = 0;
}

void SFHDS_FinishFhd(void* self) {
    *(u32*)((u8*)self + 0x00) = 0;
    *(u32*)((u8*)self + 0x0c) = 0;
    *(u32*)((u8*)self + 0x90) = 0;
}

void SFHDS_SetHdr() {}

void SFHDS_ReprocessHdr() {}

void sfhds_DoProcessHdr() {}

void sfhds_AnlyVideo() {}

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

extern char lbl_eu_80606E38[];
void* fn_803C3300(void) {
    return lbl_eu_80606E38 + 0x21c;
}
