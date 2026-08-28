// Auto-scaffolded catalog TU for CriWare/src/adx/sj/sj_utl
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

typedef struct SJ_CHUNK {
    unsigned char *ptr;
    int size;
} SJ_CHUNK;

void SJ_SplitChunk(const SJ_CHUNK *src, int size, SJ_CHUNK *dst1, SJ_CHUNK *dst2) {
    dst1->ptr = src->ptr;
    dst1->size = src->size;
    dst2->size = src->size;
    if (dst1->size > size) {
        dst1->size = size;
    }
    if ((dst2->size -= dst1->size) == 0) {
        dst2->ptr = (unsigned char *)0;
    } else {
        dst2->ptr = dst1->ptr + dst1->size;
    }
}

int strncmp(const char* s1, const char* s2, size_t n);
__declspec(section ".data") __attribute__((aligned(8))) s32 lbl_eu_80565C90[0x70] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000002, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007, 0x00000008, 0x00000009, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000000A, 0x0000000B, 0x0000000C, 0x0000000D, 0x0000000E, 0x0000000F, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000000A, 0x0000000B, 0x0000000C, 0x0000000D, 0x0000000E, 0x0000000F, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

static inline s32 sj_tag_hash(const char* s) {
    s32 h = 0;
    s32 i;
    for (i = 0; i < 7; i++)
        h = (h << 4) + lbl_eu_80565C90[s[i]];
    return h;
}

void* SJ_SearchTag(const u8** list, const char* tag, const char* tag2, u32* out) {
    u8* p;
    u8* end;
    u8* found;
    out[0] = 0;
    out[1] = 0;
    p = (u8*)list[0];
    end = p + (s32)list[1];
    while (p < end) {
        if (strncmp((const char*)p, tag, 7) == 0) {
            found = p + 16;
            out[0] = (u32)found;
            out[1] = (u32)sj_tag_hash((const char*)(p + 8));
            break;
        }
        if (tag2 != NULL && strncmp((const char*)p, tag2, 7) == 0)
            return NULL;
        s32 h = sj_tag_hash((const char*)(p + 8));
        p = (u8*)(h + (u32)p) + 16;
    }
    return (p < end) ? (void*)p : NULL;
}
