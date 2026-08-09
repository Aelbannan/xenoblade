// Auto-scaffolded catalog TU for kyoshin/cf/ICamControlClassic
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/ICamControlClassic.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

// Reads bit 4 of the classic controller input status field_0x4.
u32 func_80274EB0(ICamControlIf* self) {
    return (self->getStatus()->field_0x4 >> 27) & 1;
}

// Nonzero when both field_0x0 bit 4 and field_0x4 bit 7 are set.
u32 func_80274EE4(ICamControlIf* self) {
    ICamControlStatus* st = self->getStatus();
    return (st->field_0x0 & 0x08000000) && (st->field_0x4 & 0x01000000);
}

// Tri-state: some button pressed → 1, other combo → 0.
u32 func_80274F34(ICamControlIf* self) {
    u32 v = self->getStatus()->field_0x0;
    u32 b10 = (v >> 21) & 1;
    if (b10 && (v & 0x00400000)) return 0;   // bit 9
    return (v & 0x08000000) && b10;          // bit 4
}

// Tri-state: some button pressed → 1, other combo → 0.
u32 func_80274F94(ICamControlIf* self) {
    u32 v = self->getStatus()->field_0x0;
    if ((v & 0x00200000) && (v & 0x00400000)) return 0;   // bits 10, 9
    return (v & 0x08000000) && (v & 0x00400000);          // bits 4, 9
}

// Reads bit 12 of status field_0x0.
u32 func_80274FF4(ICamControlIf* self) {
    return (self->getStatus()->field_0x0 >> 19) & 1;
}

// Reads bit 11 of status field_0x0.
u32 func_80275028(ICamControlIf* self) {
    return (self->getStatus()->field_0x0 >> 20) & 1;
}

// Reads bit 14 of status field_0x0.
u32 func_8027505C(ICamControlIf* self) {
    return (self->getStatus()->field_0x0 >> 17) & 1;
}

// Reads bit 13 of status field_0x0.
u32 func_80275090(ICamControlIf* self) {
    return (self->getStatus()->field_0x0 >> 18) & 1;
}

// Chained OR of bit-combination conditions (see retail register chain r5→r4→r3).
u32 func_802750C4(ICamControlIf* self) {
    ICamControlStatus* st = self->getStatus();
    int a = 1;
    int b = 1;
    int c = 1;
    u32 v = st->field_0x0;
    if ((v & 0x00080000) == 0 && (v & 0x00100000) == 0) c = 0;  // bits 12, 11
    if (c == 0 && (v & 0x00020000) == 0) b = 0;                 // bit 14
    if (b == 0 && (v & 0x00040000) == 0) a = 0;                 // bit 13
    return a;
}

CPad* func_80274EA8() {
    return cf::CfGameManager::getPad(0);
}