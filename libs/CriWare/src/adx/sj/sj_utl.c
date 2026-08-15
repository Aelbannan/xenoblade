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
extern const s32 lbl_eu_80565C90[];

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
        p = (u8*)(16 + ((u32)sj_tag_hash((const char*)(p + 8)) + (u32)p));
    }
    return (p < end) ? (void*)p : NULL;
}
