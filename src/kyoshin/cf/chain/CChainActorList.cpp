#include "kyoshin/cf/chain/CChainActorList.hpp"
#include "kyoshin/cf/chain/CChainChance.hpp"
#include "kyoshin/cf/chain/CChainCombo.hpp"
#include <cstring>

// Cross-unit C-linkage callees / globals referenced by the functions below.
extern "C" void func_802A07F4(int, void*);
extern "C" u32 func_8013C54C();
extern "C" void func_8013E800(int);
extern "C" u32 lbl_eu_80662A80;

bool func_8027C1A8();

namespace cf {
    CChainActorList::CChainActorList(){

    }

    CChainActorList::~CChainActorList(){
        
    }
}

// Appends @p p to the actor list at index mCount, then increments mCount.
void func_8027C5CC(cf::CChainList* self, cf::CChainActor* p) {
    self->mActors[self->mCount++] = p;
}

// Returns the list entry whose first u32 field matches @p key.
cf::CChainActor* func_8027CA98(cf::CChainList* self, u32 key) {
    for (int i = 0; i < (int)self->mCount; i++) {
        if (self->mActors[i]->unk0 == key)
            return self->mActors[i];
    }
    return 0;
}

void func_8027B164(){}
void func_8027B200(){}
void func_8027B2CC(){}
void func_8027B770(){}
void func_8027B814(){}
void func_8027B8C8(){}
void func_8027BA0C(){}
void func_8027BB4C(){}
void func_8027BC14(){}
void func_8027BE84(){}
void func_8027BF58(){}
void func_8027EEF4(int);

void func_8027BFE0(unsigned int param) {
    if (param >= 0xBB8) {
        func_8027EEF4(0x33);
    }
    if (param >= 0x7530) {
        func_8027EEF4(0x34);
    }
    if (param >= 0x186A0) {
        func_8027EEF4(0x35);
    }
}
void func_8027C040(cf::CChainFlag* self) {
    if (self->field_0x3F00 & 2) {
        lbl_eu_80662A80++;
        if (lbl_eu_80662A80 >= 4) {
            func_8027EEF4(0x36);
        }
        if (lbl_eu_80662A80 >= 5) {
            func_8027EEF4(0x37);
        }
    }
}
void func_8027C0B0(){}
// Starts a chain message; writes 0xa to the message id on success.
bool func_8027C154(cf::CChainMsg* self) {
    if (func_8027C1A8()) {
        func_802A07F4(0xc3, 0);
        self->field_0x0 = 0xa;
        return true;
    }
    return false;
}
bool func_8027C1A8() {}
void func_8027C33C(){}
// Zeroes the actor list: clears the pointer array, count, and flag.
void func_8027C45C(cf::CChainList* self) {
    memset(self->mActors, 0, sizeof(self->mActors));
    self->mCount = 0;
    self->mFlag = 0;
}
void func_8027C49C(){}
// Function-pointer type for a CChainActor's manual vtable (mVTable at 0x70).
typedef void (*CChainActorVFn)(cf::CChainActor*);

// Calls vtable[4] on every actor in the list.
void func_8027C560(cf::CChainList* self) {
    for (int i = 0; i < (int)self->mCount; i++) {
        cf::CChainActor* a = self->mActors[i];
        ((CChainActorVFn*)a->mVTable)[4](a);
    }
}
void func_8027C5E4(){}
void func_8027C6B4(){}
void func_8027C924(){}
void func_8027CA0C(){}
void func_8027CAE0(){}
// If the counter is positive, runs chain update steps and resets it.
void func_8027CBE8(cf::CChainCounter* self) {
    if (self->field_0x0 > 0) {
        func_802AA338__Fv();
        func_8013C54C();
        func_8013E800(self->field_0x0);
        func_8027BFE0(self->field_0x0);
        self->field_0x0 = 0;
    }
}
void func_8027CC3C(){}
void func_8027CD08(){}
