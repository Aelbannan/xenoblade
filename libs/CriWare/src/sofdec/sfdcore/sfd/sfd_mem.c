// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_mem
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

int SFMEM_Init(void) { return 0x0; }

int SFMEM_Finish(void) { return 0x0; }

int SFMEM_ExecServer(void* p) {
    unsigned int val = *(unsigned int*)((char*)p + 0x1fec);
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
void* SFMEM_GetWrite(void* self, u32 param) {
    return SFBUF_RingGetWrite(self, *(u32*)((u8*)self + 0x1fec), param);
}

void SFBUF_RingAddWrite(void* self, u32 a, u32 b, u32 c);
void SFMEM_AddWrite(void* self, u32* a, u32* b) {
    SFBUF_RingAddWrite(self, *(u32*)((u8*)self + 0x1fec), *a, *b);
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
