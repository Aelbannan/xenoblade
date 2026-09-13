// CEvent1 - Array-indexed accessor functions

#include <types.h>

// Large class (size at least 0x1108+)
// Has an array of pointers at offset 0x1104. Slots are polymorphic
// (floats, vecs, …); stored as u8* so the TU isn't void*-typed.
typedef struct {
    u8 pad[0x1104];          // padding to reach array offset
    u8* mPtrArray[1];        // 0x1104 - flexible array of pointers
} CEvent1;

// extern "C" to produce the exact Fv mangling
extern "C" {

float getFloatParam__7CEvent1Fv(CEvent1* ths, int index) {
    return *reinterpret_cast<f32*>(ths->mPtrArray[index]);
}

// Return type void* is the retail call-site ABI (callers cast).
void* getPtrParam__7CEvent1Fv(CEvent1* ths, int index) {
    return ths->mPtrArray[index];
}

}
