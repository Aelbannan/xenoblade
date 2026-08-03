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

extern s32 SFH_AnlyElemCodecVid(void*, u8, s32*);
extern s32 SFH_AnlyElemBitRate(void*, u8, s32*);
extern s32 SFH_AnlyElemPicSz(void*, u8, s32*, s32*);
extern s32 SFH_AnlyElemPicRate(void*, u8, s32*);
extern s32 SFH_IsEffFtrInf(void*, u8, s32*);
extern s32 SFH_AnlyFtrColType(void*, u8, s32*);
extern s32 SFH_AnlyFtrPicType(void*, u8, s32*);
extern s32 SFH_AnlyFtrFixFlg(void*, u8, s32*);
extern s32 SFH_AnlyFtrShcFixFlg(void*, u8, s32*);
extern s32 SFH_AnlyFtrExpand(void*, u8, s32*);
extern s32 SFH_AnlyFtrGopN(void*, u8, s32*);
extern s32 SFH_AnlyFtrGopM(void*, u8, s32*);

void sfhds_AnlyVideo(void* self, u32 a, s32* out) {
    s32 tEff, tCodec, tBitRate, tPicRate, tColType, tPicType, tFixFlg, tShcFixFlg, tExpand, tGopN, tGopM;
    s32 v;

    if (SFH_AnlyElemCodecVid(self, (u8)a, &tCodec) == 0)
        v = -1;
    else
        v = tCodec;
    out[0] = v;

    if (SFH_AnlyElemBitRate(self, (u8)a, &tBitRate) == 0)
        v = -1;
    else
        v = tBitRate;
    out[1] = v;

    if (SFH_AnlyElemPicSz(self, (u8)a, &out[2], &out[3]) == 0) {
        out[2] = -1;
        out[3] = -1;
    }

    if (SFH_AnlyElemPicRate(self, (u8)a, &tPicRate) == 0)
        v = -1;
    else
        v = tPicRate;
    out[4] = v;

    if (SFH_IsEffFtrInf(self, (u8)a, &tEff) == 0)
        tEff = 0;
    out[5] = (tEff != 0);
    if (tEff == 0)
        return;

    if (SFH_AnlyFtrColType(self, (u8)a, &tColType) == 0)
        v = -1;
    else
        v = tColType;
    out[6] = v;

    if (SFH_AnlyFtrPicType(self, (u8)a, &tPicType) == 0)
        v = -1;
    else
        v = tPicType;
    out[7] = v;

    if (SFH_AnlyFtrFixFlg(self, (u8)a, &tFixFlg) == 0)
        v = -1;
    else
        v = tFixFlg;
    out[8] = v;

    if (SFH_AnlyFtrShcFixFlg(self, (u8)a, &tShcFixFlg) == 0)
        v = -1;
    else
        v = tShcFixFlg;
    out[9] = v;

    if (SFH_AnlyFtrExpand(self, (u8)a, &tExpand) == 0)
        v = -1;
    else
        v = tExpand;
    out[10] = v;

    if (SFH_AnlyFtrGopN(self, (u8)a, &tGopN) == 0)
        v = -1;
    else
        v = tGopN;
    out[11] = v;

    if (SFH_AnlyFtrGopM(self, (u8)a, &tGopM) == 0)
        v = -1;
    else
        v = tGopM;
    out[12] = v;
}

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
