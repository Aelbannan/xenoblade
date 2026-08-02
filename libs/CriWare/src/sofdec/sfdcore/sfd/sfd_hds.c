// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_hds
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>

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

extern void* SFH_Create(u32 arg0, s32 arg1, void* ctx);
extern s32 SFH_Destroy(void* ctx);
void sfhds_DoProcessHdr(void* a, void* b);
extern u32 SFMPS_GetConcatCnt(void* self);
extern s32 SFLIB_SetErr(s32 val, u32 err_code);

s32 SFHDS_ReprocessHdr(void* self) {
    u8* hdr = *(u8**)((u8*)self + 0x2670);
    u8 ctx[0x18];
    void* created;
    u8* src;
    s32 ret = 0;

    if (hdr == NULL)
        return 0;
    if (SFMPS_GetConcatCnt(self) > 0)
        return 0;
    src = hdr + 0xC;
    if (src == NULL)
        return 0;

    {
        u32* d = (u32*)((u8*)self + 0x88);
        u32* s = (u32*)src;
        s32 n = 274;
        do {
            *d++ = *s++;
            *d++ = *s++;
        } while (--n != 0);
        *(u32*)d = *(u32*)s;
    }

    created = SFH_Create((u32)((u8*)self + 0x11C), *(s32*)((u8*)self + 0x118), ctx);
    if (created == NULL) {
        ret = SFLIB_SetErr(0, 0xff000232);
    } else {
        sfhds_DoProcessHdr(created, (u8*)self + 0x88);
        ret = SFH_Destroy(created);
    }

    *(s32*)((u8*)self + 0x940) = *(s32*)((u8*)self + 0xac);
    *(s32*)((u8*)self + 0x944) = *(s32*)((u8*)self + 0xb0);
    *(s32*)((u8*)self + 0x948) = *(s32*)((u8*)self + 0xb4);
    return ret;
}

void sfhds_DoProcessHdr(void* a, void* b) {} // stub

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
