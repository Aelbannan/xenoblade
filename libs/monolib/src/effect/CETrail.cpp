// Auto-scaffolded catalog TU for monolib/src/effect/CETrail
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <revolution/MTX.h>
#include <decomp.h>

struct CETrail {
    ~CETrail();
};

// Forward declarations
void func_804EE60C(void*);
void* func_80494128(void*);
void func_80494188(void*);
void func_804C03A0(void*, int);
void func_804C08C8(void*, int);
void __dl__FPv(void*);

void __dt__804D6C60(){}

// func_804D70A0: initializes a struct with two zero Vec3s and a float
// Retail: copies zero__Q22ml5CVec3 to offset 0x08 and 0x14, stores float at 0x20
struct CVec3 { float x, y, z; };
extern CVec3 zero__Q22ml5CVec3;
void func_804D70A0(void* obj) {
    volatile float stack_temp[8];
    char* p = (char*)obj;
    float* fp = (float*)&zero__Q22ml5CVec3;
    float zero_val = 0.0f;
    float f3 = fp[0], f2 = fp[1], f1 = fp[2];
    stack_temp[0] = f3; stack_temp[1] = f2; stack_temp[2] = f1;
    int* ip = (int*)stack_temp;
    int* dst1 = (int*)(p + 0x08);
    dst1[0] = ip[0]; dst1[1] = ip[1]; dst1[2] = ip[2];
    stack_temp[0] = f3; stack_temp[1] = f2; stack_temp[2] = f1;
    int* dst2 = (int*)(p + 0x14);
    dst2[0] = ip[0]; dst2[1] = ip[1]; dst2[2] = ip[2];
    *(float*)(p + 0x20) = zero_val;
}

void __dt___reslist_base_CETrail_POINT(){}

void __dt__reslist_CETrail_POINT(){}

CETrail::~CETrail() {}

// func_804D73FC: calls func_804EE60C on two member sub-objects
void func_804D73FC(void* self) {
    func_804EE60C((char*)self + 0xC8);
    func_804EE60C((char*)self + 0x110);
}

void func_804D7434(){}

void func_804D77E4(){}

void func_804D7B28(){}

// func_804D807C: init with resource acquisition
void* func_804D807C(void* self, void* obj) {
    *(void**)self = obj;
    void* result = func_80494128(*(void**)((char*)obj + 0x5C));
    *(void**)((char*)self + 0x04) = result;
    if (!result) return self;
    func_804C03A0(result, 3);
    void* r3 = *(void**)((char*)self + 0x04);
    u32 val = *(u32*)((char*)r3 + 0x30);
    val = DECOMP_PPC_RLWINM(val, 0, 0, 27);
    val = val | 0xF;
    *(u32*)((char*)r3 + 0x30) = val;
    func_804C08C8(*(void**)((char*)self + 0x04), 0);
    return self;
}

// __dt__804D80F0: destructor that releases a resource
void __dt__804D80F0(void* self, int destroy_flag) {
    if (self) {
        void** slot = (void**)((char*)self + 0x04);
        if (*slot) {
            func_80494188(**(void***)((char*)self + 0x00));
            *slot = nullptr;
        }
        if (destroy_flag > 0) {
            __dl__FPv(self);
        }
    }
}

void func_804D8160(){}

void func_804D82DC(){}

void func_804D83D0(){}
