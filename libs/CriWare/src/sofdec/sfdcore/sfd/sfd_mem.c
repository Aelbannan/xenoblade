// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_mem
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"

int SFMEM_Init(void) { return 0x0; }

int SFMEM_Finish(void) { return 0x0; }

int SFMEM_ExecServer(SfdHandle* p) {
    unsigned int val = p->trn[0].termIdx;
    SFBUF_SetPrepFlg(p, val, 1);
    return 0;
}

int SFMEM_Create(void) { return 0x0; }

int SFMEM_Destroy(void) { return 0x0; }

int SFMEM_RequestStop(void) { return 0x0; }

int SFMEM_Start(void) { return 0x0; }

int SFMEM_Stop(void) { return 0x0; }

int SFMEM_Pause(void) { return 0x0; }

void* SFBUF_RingGetWrite(void* self, u32 field, u32 param);
void* SFMEM_GetWrite(SfdHandle* self, u32 param) {
    return SFBUF_RingGetWrite(self, self->trn[0].termIdx, param);
}

void SFBUF_RingAddWrite(void* self, u32 a, u32 b, u32 c);
void SFMEM_AddWrite(SfdHandle* self, u32* a, u32* b) {
    SFBUF_RingAddWrite(self, self->trn[0].termIdx, *a, *b);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFMEM_GetRead(void* h) {
    return SFLIB_SetErr(h, 0xff000501);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFMEM_AddRead(void* h) {
    return SFLIB_SetErr(h, 0xff000501);
}

int SFMEM_Seek(void) { return 0x0; }

/* JP map: SFD_tr_in_mem (memory-input transfer module interface) */
const SfdTrIf SFD_tr_in_mem = {
    SFMEM_Init, SFMEM_Finish, SFMEM_ExecServer, SFMEM_Create,
    SFMEM_Destroy, SFMEM_RequestStop, SFMEM_Start, SFMEM_Stop,
    SFMEM_Pause, SFMEM_GetWrite, SFMEM_AddWrite, SFMEM_GetRead,
    SFMEM_AddRead, SFMEM_Seek, NULL,
};
