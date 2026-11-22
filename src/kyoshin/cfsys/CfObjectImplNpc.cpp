// Auto-scaffolded catalog TU for kyoshin/cfsys/CfObjectImplNpc
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cfsys/CfObjectImplNpc.hpp"
#include "kyoshin/code_800AA008_Tok.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

// name-guess: retail .data pointer table names the ten NPC resource keys.
const char* NpcResNames[10] = {
    "np02030", "np63010", "np66010", "np67010", "np71010",
    "np73010", "np76010", "np77010", "np81010", "np45080",
};

// name-guess: retail .rodata table holds the ten NPC talk volume levels.
static const f32 sNpcTalkValues[10] = {
    0.4f, 0.4f, 0.4f, 0.4f, 0.4f,
    0.4f, 0.4f, 0.4f, 0.4f, 0.1f,
};

// us-800cb118: NPC init - reset the talk index to -1, prepare via
// MoveImplInitFields, bind the +0x28 voice sub-object to the +0x14 driver, then
// scan the driver's resource-name string for one of the 10 names in
// NpcResNames; on a hit dispatch the vtable 0x30 command and record the
// name index in field_368.
void NpcImplInit(cf::CfObjectImplNpc* self)
{
    self->field_368 = -1;
    MoveImplInitFields(self);
    if ((self->field_14->field_64 & 0x80000000) != 0) {
        attachOwner(&self->field_28, self->field_14);
    }
    if (self->field_14 != 0) {
        char* name = Tok_FormatRes((void*)self->field_14->field_70);
        for (u32 i = 0; i < 10; i++) {
            if (strstr(name, NpcResNames[i]) != 0) {
                self->vf30(0x400, 1);
                self->field_368 = (s16)i;
                break;
            }
        }
    }
}

// us-800cb1e8: NPC per-frame update - dispatch vtable 0xDC, then while the
// +0x90 word is set, feed the float table value at the talk index into the
// +0x98 sub-object's vtable 0x5c; finally refresh the +0x28 voice sub-object
// when the +0x64 MSB (sign bit) is set.
void NpcImplUpdate(cf::CfObjectImplNpc* self)
{
    if (self->field_14 != 0) {
        self->vfDC();
        if (self->field_14->field_90 != 0) {
            s16 idx = self->field_368;
            if (idx >= 0) {
                self->field_14->field_98->vf5C(sNpcTalkValues[idx]);
            }
            if ((self->field_14->field_64 & 0x80000000) != 0) {
                updatePosition(&self->field_28);
            }
        }
    }
}

void NpcImplNoopA(void) {}

// us-800cb288: NPC per-frame update - runs MoveImplStartDispatch, and when the +0x70
// token formats to entry id 2 / param 8 past scenario 0x167, notifies the
// +0x98 sub-object with the "core" string.
void NpcImplNotifyEvent(cf::CfObjectImplNpc* self)
{
    MoveImplStartDispatch(self);
    u32 out14;
    u32 out10;
    u32 outC;
    u32 out8;
    Tok_Unpack(self->field_14->field_70, &out14, &out10, &outC, &out8);
    if (out14 == 2 && out10 == 8) {
        if ((u32)cf::CfGameManager::getQueuedFileEventCount() >= 0x167) {
            self->field_14->field_98->vf28("core", 0);
        }
    }
}

extern "C" void* __dt__Q22cf15CfObjectImplNpcFv(cf::CfObjectImplNpc* self,
                                               int deleteFlag) {
    if (self != nullptr && deleteFlag > 0) {
        operator delete(self);
    }
    return self;
}

u8* cf::CfObjectImplNpc::NpcImplGetUnk68() { return this->mUnk_0x68; }

void NpcImplNoopB() {}

void NpcImplNoopC() {}

void NpcImplNoopD() {}

extern "C" void NpcImplDtorThunkC(void* self) { ((void(*)(void*))__dt__Q22cf15CfObjectImplNpcFv)((char*)self - 0xc); }

extern "C" void NpcImplDtorThunk10(void* self) { ((void(*)(void*))__dt__Q22cf15CfObjectImplNpcFv)((char*)self - 0x10); }
