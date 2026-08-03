// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_slice
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void MPVSL_Init(void) {}

void MPVSL_Finish(void) {}

void MPVSL_Create(void) {}

void MPVSL_Destroy(void) {}

typedef struct MpvSjChunk2 {
    const u8* p;
    s32 size;
} MpvSjChunk2;

extern s32 MPV_CheckDelim(const u8* p);
extern s32 MPVSL_DecSliceOne(void* self, s32 a, void* sj, s32* out);
extern void MPVHDEC_RecoverSj(void* self, void* sj);

s32 MPVSL_DecPicture(void* self, void* sj) {
    MpvSjChunk2 st;
    s32 total = 0;
    s32 maxCode = 257;
    s32 n;
    for (;;) {
        s32 r;
        if (*(s32*)((u8*)self + 0xDA8) != 0)
            return 0;
        r = MPVSL_DecSliceOne(self, -1, sj, NULL);
        if (r != 0) {
            MPVHDEC_RecoverSj(self, sj);
            return 0;
        }
        ((void (*)(void*, s32, s32, MpvSjChunk2*))*(void**)((char*)*(void**)sj + 0x18))(
            sj, 1, 0x7FFFFFFF, &st);
        ((void (*)(void*, s32, MpvSjChunk2*))*(void**)((char*)*(void**)sj + 0x1C))(
            sj, 1, &st);
        if (st.size >= 4 && (MPV_CheckDelim(st.p) & 1)) {
            s32 code = (s32)*(u8*)((u8*)st.p + 3) | 0x100;
            if (maxCode <= code) {
                (*(s32*)((u8*)self + 0xBE8))++;
            } else {
                maxCode = code;
                (*(s16*)((u8*)self + 0xC30))++;
                MPVSL_DecSliceOne(self, 0, sj, &n);
                total += n;
                if (*(s32*)((u8*)self + 0xBE8 + 0x1CC) == 0)
                    continue;
            }
        }
        break;
    }
    if (*(s32*)((u8*)self + 0xCD8) != *(s32*)((u8*)self + 0xCE4)) {
        (*(s32*)((u8*)self + 0xBE8))++;
    }
    if (total != 0) {
        *(s32*)((u8*)self + 0xBE8) += total;
    }
    return 0;
}

s32 MPVSL_DecSliceOne(void* self, s32 a, void* sj, s32* out) { return 0; }
