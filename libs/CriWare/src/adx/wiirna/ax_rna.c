// Auto-scaffolded catalog TU for CriWare/src/adx/wiirna/ax_rna
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
void AXRNA_EntryErrFunc(void* fn) { RNAERR_EntryErrFunc(fn); }


extern u32 lbl_eu_805F2C00;
extern u32 lbl_eu_8051914C;
extern u8 lbl_eu_805F2C08[];

void AXRNA_Init(void) {
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

void axrna_voice_drop() {}

void AXRNA_Create() {}

void AXRNA_SetTransSw() {}

void AXRNA_SetPlaySw() {}

void AXRNA_GetNumData() {}

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

void AXRNA_SetOutVol() {}

void AXRNA_SetOutPan() {}

void AXRNA_SetBitPerSmpl(void* self, u32 val) {
    if (self != NULL) {
        *(u32*)((u8*)self + 0x78) = val;
    }
}

int AXRNA_SetStmHdInfo(void) { return 0x0; }

int AXRNA_DiscardData(void) { return 0x0; }

void AXRNA_SetMain() {}

void criware_8039A8E0() {}
