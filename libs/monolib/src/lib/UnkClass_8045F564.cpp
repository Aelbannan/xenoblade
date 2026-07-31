// Decompiled target(s):
//   - us-80463550 UnkClass_8045F564::~UnkClass_8045F564()
//   - us-80463650 UnkClass_8045F564::createRegion(int, int, const char*, int)
//   - us-80463748 UnkClass_8045F564::func_8045F778()
//   - us-804637b8 UnkClass_8045F564::func_8045F7E8()
//   - us-804637e0 UnkClass_8045F564::func_8045F810()

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util/MemManager.hpp"
#include <revolution/mem.h>

extern "C" {
extern CLibLayout* lbl_eu_80665710;
extern const char lbl_eu_805231BC[];
}

using namespace mtl;

UnkClass_8045F564::UnkClass_8045F564()
    : unk0(-1), unk4(0), unk8(0), unkC(0) {}

UnkClass_8045F564::~UnkClass_8045F564() {
    if (unk0 != 0xFFFFFFFF) {
        if (unk4 != 0) {
            MEMDestroyFrmHeap(reinterpret_cast<MEMiHeapHead*>(unk4));
            unk4 = 0;
        }
        MemManager::func_80434A4C(false);
        MemManager::erase(unk0);
        MemManager::func_80434A4C(true);
        unk0 = 0xFFFFFFFF;
    }
    
    // Remove this from CLibLayout tracking array.
    // Opaque byte-offset arithmetic intentionally mirrors retail codegen:
    // struct-field access would let MWCC cache instanceCount across the
    // shift loop, but retail reloads the singleton from the global each
    // iteration (stores through a pointer derived from the same global).
    u8* base = (u8*)lbl_eu_80665710;
    u32 cnt = *(u32*)(base + 0x2B8);
    u32 i = 0;
    u8* it = base;
    while (i < cnt) {
        if (*(u32*)(it + 0x238) == (u32)this) {
            u32 bo = i * 4;
            u32 curCnt;
            while (i < (curCnt = *(u32*)((u8*)lbl_eu_80665710 + 0x2B8)) - 1) {
                u8* cur = (u8*)lbl_eu_80665710 + bo;
                bo += 4;
                u32 next = *(u32*)(cur + 0x23C);
                i++;
                *(u32*)(cur + 0x238) = next;
            }
            *(u32*)((u8*)lbl_eu_80665710 + 0x2B8) = curCnt - 1;
            break;
        }
        it += 4;
        i++;
    }
}

void UnkClass_8045F564::createRegion(int size1, int size2, const char* name, int flag) {
    MemManager::func_80434A4C(false);
    
    if (unk0 != 0xFFFFFFFF) {
        MemManager::erase(unk0);
    }
    
    if (name != nullptr) {
        if (flag != 0) {
            unk0 = MemManager::create_tail(size1, size2, name);
        } else {
            unk0 = MemManager::create(size1, size2, name);
        }
    } else {
        if (flag != 0) {
            unk0 = MemManager::create_tail(size1, size2, lbl_eu_805231BC + 0xB);
        } else {
            unk0 = MemManager::create(size1, size2, lbl_eu_805231BC + 0xB);
        }
    }
    
    MemManager::func_80434A4C(true);
    
    void* allocData = MemManager::getMaxAllocData(unk0);
    unk4 = reinterpret_cast<u32>(MEMCreateFrmHeapEx(allocData, size2, 0));
}

void UnkClass_8045F564::func_8045F778() {
    if (unk0 == 0xFFFFFFFF) return;
    
    if (unk4 != 0) {
        MEMDestroyFrmHeap(reinterpret_cast<MEMiHeapHead*>(unk4));
        unk4 = 0;
    }
    MemManager::func_80434A4C(false);
    MemManager::erase(unk0);
    MemManager::func_80434A4C(true);
    unk0 = 0xFFFFFFFF;
}

void UnkClass_8045F564::func_8045F7E8() {
    if (unk0 == 0xFFFFFFFF) return;
    if (unk4 == 0) return;
    MEMFreeToFrmHeap(reinterpret_cast<MEMiHeapHead*>(unk4), 3);
}

void UnkClass_8045F564::func_8045F810() {
    MemManager::func_804348A4(unk0, 1);
    if (unk4 != 0) {
        MEMGetAllocatableSizeForFrmHeapEx(reinterpret_cast<MEMiHeapHead*>(unk4), 4);
    }
}
