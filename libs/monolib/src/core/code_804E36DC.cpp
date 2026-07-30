// Auto-scaffolded catalog TU for monolib/src/core/code_804E36DC
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>

// External functions referenced by this TU
extern "C" void func_804E536C();
extern "C" void func_804DF2F0(u32 a, u32 b);

void func_804E36DC(){}

void func_804E39E8(){}

void func_804E3B08(){}

void func_804E3B6C(){}

// us-804e8168: func_804E3CCC - clear bit 17 of field_0x0, then tail-call func_804E536C
void func_804E3CCC(void* r3) {
    u16* field_0 = (u16*)r3;
    *field_0 &= ~(1 << 17);
    func_804E536C();
}

// us-804e8178: func_804E3CDC - store floats if non-zero
void func_804E3CDC(void* r3, float f1, float f2) {
    if (f1 == 0.0f) return;
    if (f2 == 0.0f) return;
    float* field_0x90 = (float*)((u8*)r3 + 0x90);
    field_0x90[0] = f1;
    field_0x90[1] = f2;
}

// us-804e8198: func_804E3CFC - tail-call func_804DF2F0(field_0xC, field_0x4)
void func_804E3CFC(void* r3) {
    u32* fields = (u32*)r3;
    func_804DF2F0(fields[3], fields[1]);
}

// us-804e81a8: func_804E3D0C - find first empty slot in field_0xDC[4] and store r4
void func_804E3D0C(void* r3, u32 r4) {
    u32* field_0xDC = (u32*)((u8*)r3 + 0xDC);
    for (int i = 0; i < 4; i++) {
        if (field_0xDC[i] == 0) {
            field_0xDC[i] = r4;
            return;
        }
    }
}

// us-804e81e4: func_804E3D48 - find slot matching r4 in field_0xDC[4] and clear it
void func_804E3D48(u8* r3, u32 r4) {
    u8* r5 = r3;
    for (int r6 = 0; r6 < 4; r6++) {
        if (*(u32*)(r5 + 0xDC) == r4) {
            *(u32*)(r3 + r6 * 4 + 0xDC) = 0;
            return;
        }
        r5 += 4;
    }
}

void func_804E3D88(){}

void func_804E3E2C(){}

void func_804E3EB4(){}

void func_804E3EF4(){}

void func_804E3FB0(){}

void func_804E4094(){}

void func_804E41B4(){}

void func_804E424C(){}

void __dt__804E42F4(){}
