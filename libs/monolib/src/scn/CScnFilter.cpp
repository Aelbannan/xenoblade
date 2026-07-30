// Translation unit: monolib/src/scn/CScnFilter
// CScnFilter - scene filter base class.

#include <types.h>
#include <monolib/scn/CScnFilter.hpp>

// External symbols
extern "C" void* lbl_eu_8056EB60; // CScnFilter vtable
extern float lbl_eu_8066ABA0;
extern "C" void __dl__FPv(void*);

// Constructor: initializes vtable and all members to default values.
CScnFilter::CScnFilter() {
    *(void**)this = (void*)&lbl_eu_8056EB60;
    mEnabled = 0;
    mFlags = 3;
    mUnk0C = 0;
    mIntensity = lbl_eu_8066ABA0;
    mUnk14 = 0;
}

// Destructor: standard deleting destructor pattern.
CScnFilter::~CScnFilter() {
}

// func_8049C868: update filter intensity and trigger callback on threshold.
// Takes (this, arg) where arg+0x84 points to an object with a float at 0x0C.
void CScnFilter::func_8049C868(void* arg) {
    if (mUnk0C == 0) {
        return;
    }

    // Check if intensity already exceeds threshold
    bool over = false;
    if (mUnk0C != 0) {
        double threshold = (double)mUnk0C;
        if (mIntensity > threshold) {
            over = true;
        }
    }
    if (over) {
        return;
    }

    // Increment intensity by delta from arg->0x84->0x0C
    void* obj = *(void**)((char*)arg + 0x84);
    mIntensity += *(float*)((char*)obj + 0x0C);

    // Check if intensity now exceeds threshold
    if (mUnk0C != 0) {
        double threshold = (double)mUnk0C;
        if (mIntensity > threshold) {
            // Trigger callback via vtable at mUnk14
            if (mUnk14 != 0) {
                void* cb = mUnk14;
                void** vtbl = *(void***)cb;
                void (*fn)(void*) = (void (*)(void*))vtbl[3];
                fn(cb);
            }
        }
    }
}
