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

extern CLibLayout* lbl_eu_80665710;
extern const char lbl_eu_805231BC[];

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
    
    // Remove this instance from CLibLayout's tracking array by shifting
    // every later entry down one slot.
    CLibLayout* lib = lbl_eu_80665710;
    for (u32 i = 0; i < lib->instanceCount; i++) {
        if (lib->instanceArray[i] != this) continue;

        while (i < lib->instanceCount - 1) {
            lib->instanceArray[i] = lib->instanceArray[i + 1];
            i++;
        }
        lib->instanceCount--;
        break;
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
