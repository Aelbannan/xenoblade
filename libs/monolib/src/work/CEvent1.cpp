// CEvent1 - Array-indexed accessor functions

#include <types.h>

// Large class (size at least 0x1108+)
// Has an array of pointers at offset 0x1104
typedef struct {
    u8 pad[0x1104];          // padding to reach array offset
    void* mPtrArray[1];      // 0x1104 - flexible array of pointers
} CEvent1;

// extern "C" to produce the exact Fv mangling
extern "C" {

float func_8043B574__7CEvent1Fv(CEvent1* self, int index) {
    void* ptr = self->mPtrArray[index];
    return *(float*)ptr;
}

void* func_8043B588__7CEvent1Fv(CEvent1* self, int index) {
    return self->mPtrArray[index];
}

}
