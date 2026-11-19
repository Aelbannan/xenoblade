#pragma once

#include <types.h>

/* User-operation channel record: four payload words copied by the buffer
 * unit into the UOCH slot of an SFBUF handle. */
typedef struct SfbufUoch {
    u32 w[4];
} SfbufUoch;

/* Declared by the owning sfd_buf unit; the data pointer is the four-word
 * record, expressed as u32* at the C ABI boundary. */
void SFBUF_SetUoch(void* self, int idx, int sub_idx, u32* src);
void SFBUF_GetUoch(void* self, int idx, int sub_idx, u32* dst);
