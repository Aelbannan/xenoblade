// Auto-scaffolded catalog TU for CriWare/src/adx/sj/sj_utl
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803997d4
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
// LLM-HARNESS-END: us-803997d4

// LLM-HARNESS-BEGIN: us-8039982c
void SJ_SearchTag() {}
// LLM-HARNESS-END: us-8039982c
