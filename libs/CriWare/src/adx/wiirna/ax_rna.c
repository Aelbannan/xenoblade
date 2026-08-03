// Auto-scaffolded catalog TU for CriWare/src/adx/wiirna/ax_rna
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
void AXRNA_EntryErrFunc(void* fn) { RNAERR_EntryErrFunc(fn); }


extern u32 lbl_eu_805F2C00;
extern volatile u32 lbl_eu_8051914C;
extern u8 lbl_eu_805F2C08[];

void AXRNA_Init(void) {
    (void)lbl_eu_8051914C;
    if (lbl_eu_805F2C00 == 0) {
        memset(lbl_eu_805F2C08, 0, 0xE40);
    }
    lbl_eu_805F2C00++;
}

extern void AXRNA_Destroy(void* entry);

void AXRNA_Finish(void) {
    s32 i;
    if (--lbl_eu_805F2C00 == 0) {
        for (i = 0; i < 0x10; i++) {
            if ((s32)lbl_eu_805F2C08[i * 0xE4] == 1) {
                AXRNA_Destroy(&lbl_eu_805F2C08[i * 0xE4]);
            }
        }
        memset(lbl_eu_805F2C08, 0, 0xE40);
    }
}

extern void MIXReleaseChannel(s32 ch);

void axrna_voice_drop(void* voice) {
    s32 i, j;
    for (i = 0; i < 0x10; i++) {
        for (j = 0; j < 2; j++) {
            if (voice == *(void**)((u8*)&lbl_eu_805F2C08 + i * 0xE4 + j * 4 + 8)) {
                MIXReleaseChannel(*(s32*)((u8*)&lbl_eu_805F2C08 + i * 0xE4 + j * 4 + 8));
                *(u32*)((u8*)&lbl_eu_805F2C08 + i * 0xE4 + j * 4 + 8) = 0;
                return;
            }
        }
    }
}

void AXRNA_Create() {}

void AXRNA_SetTransSw() {}

void AXRNA_SetPlaySw() {}

s32 AXRNA_GetNumData(void* self) {
    if (self == NULL)
        return -1;
    {
        u8* e = (u8*)self + (((s32)((s8)*(u8*)((u8*)self + 3) - 1) << 2) & 0xFFFFFFFC);
        void* p = *(void**)((u8*)e + 0x30);
        void* q = *(void**)((u8*)p + 0);
        u32 r = (u32)((s32 (*)(void*, s32, void*))*(void**)((u8*)q + 36))(p, 0, q);
        return 0x1000 - (s32)(r >> 1) - *(s32*)((u8*)self + 0x74);
    }
}

typedef struct AXRNA { char pad0[3]; signed char type; char pad4[0x2c]; void *objs[1]; } AXRNA; int AXRNA_GetNumRoom(AXRNA *rna) { if (rna == NULL) { return -1; } void *obj = rna->objs[rna->type - 1]; int (*func)(void *, int) = ((int (**)(void *, int))(*(void ***)obj))[9]; return (unsigned int)func(obj, 0) >> 1; }

#pragma auto_inline off
void axrna_update_play(void* self) {}

void axrna_start_trans(void* self) {}

void criware_80399F4C(void* self) {}

void axrna_start_flash(void* self) {}
#pragma auto_inline on

extern void GCRNA_LockCs(void);
extern void GCRNA_UnlockCs(void);
void AXRNA_ExecServer(void) {
    u8* rna = (u8*)lbl_eu_805F2C08;
    s32 st;

    GCRNA_LockCs();
    if ((s32)rna[0] != 1)
        goto end;
    if (rna == NULL)
        goto end;
    st = (rna != NULL) ? ((rna[1] >> 1) & 1) : -1;
    if (st == 1)
        axrna_update_play(rna);
    st = (rna != NULL) ? (s32)(rna[1] & 1) : -1;
    if (st == 1) {
        if (*(u32*)(rna + 0xB4) == 1)
            criware_80399F4C(rna);
        else
            axrna_start_trans(rna);
    } else {
        st = (rna != NULL) ? (s32)((rna[1] >> 1) & 1) : -1;
        if (st == 1 && *(u32*)(rna + 0x74) < *(u32*)(rna + 0x18))
            axrna_start_flash(rna);
    }
end:
    GCRNA_UnlockCs();
}

void AXRNA_SetNumChan(void* self, u8 numChan) {
    if (self != NULL) {
        *(u8*)((u8*)self + 0x03) = numChan;
    }
}

void AXRNA_SetSfreq() {}

extern void GCRNA_LockCs(void);
extern void GCRNA_UnlockCs(void);
extern void MIXSetInput(void* a, s32 b);

void AXRNA_SetOutVol(void* self, s32 vol) {
    s32 i;
    s32 v;
    if (self == NULL)
        return;
    v = -960;
    if (*(s32*)((u8*)self + 0x7c) == v)
        return;
    *(s32*)((u8*)self + 0x7c) = v;
    for (i = 0; i < (s8)*(u8*)((u8*)self + 2); i++) {
        void* e = *(void**)((u8*)self + 4 * i + 0);
        GCRNA_LockCs();
        if (e != NULL)
            MIXSetInput(e, v);
        GCRNA_UnlockCs();
    }
}

extern void MIXSetPan(void* a, s32 b);
extern u32 lbl_eu_80566054[];

void AXRNA_SetOutPan(void* self, s32 index, s32 pan) {
    s32 v;
    if (index >= (s8)*(u8*)((u8*)self + 2))
        return;
    if (pan > 0xf)
        pan = 0xf;
    if (pan < -0xf)
        pan = -0xf;
    if (*(s32*)((u8*)self + 0x80) == pan)
        return;
    *(s32*)((u8*)self + 0x80) = pan;
    GCRNA_LockCs();
    {
        void* e = *(void**)((u8*)self + index * 4 + 8);
        if (e != NULL) {
            v = (s32)lbl_eu_80566054[pan + 0xf];
            MIXSetPan(e, v);
        }
    }
    GCRNA_UnlockCs();
}

void AXRNA_SetBitPerSmpl(void* self, u32 val) {
    if (self != NULL) {
        *(u32*)((u8*)self + 0x78) = val;
    }
}

int AXRNA_SetStmHdInfo(void) { return 0x0; }

int AXRNA_DiscardData(void) { return 0x0; }

extern void MIXRmtSetFader(void* a, s32 b);
extern u32 lbl_eu_80519150[];

void AXRNA_SetMain(void* self, s32 index, s32 val) {
    s32 v;
    s32 i;
    if (self == NULL)
        return;
    if (index > 3) {
        RNAERR_CallErrFunc(lbl_eu_80519150 + 0x4e4);
        return;
    }
    v = -960;
    if (*(s32*)((u8*)self + index * 4 + 0xd0) == v)
        return;
    *(s32*)((u8*)self + index * 4 + 0xd0) = v;
    for (i = 0; i < (s8)*(u8*)((u8*)self + 2); i++) {
        void* e = *(void**)((u8*)self + 4 * i + 8);
        GCRNA_LockCs();
        if (e != NULL)
            MIXRmtSetFader(e, v);
        GCRNA_UnlockCs();
    }
}

void criware_8039A8E0() {}
