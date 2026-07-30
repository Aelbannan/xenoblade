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
    
    // Remove this from CLibLayout tracking array
    CLibLayout* layout = lbl_eu_80665710;
    u32 count = layout->instanceCount;
    if (count != 0) {
        u32 i = 0;
        while (i < count - 1) {
            if (layout->instanceArray[i] == this) {
                layout->instanceArray[i] = layout->instanceArray[i + 1];
                i++;
                while (i < count - 1) {
                    layout->instanceArray[i] = layout->instanceArray[i + 1];
                    i++;
                }
                layout->instanceCount = count - 1;
                goto done;
            }
            i++;
        }
    }
done:
    return;
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
        const char* defaultName = lbl_eu_805231BC + 0xB;
        if (flag != 0) {
            unk0 = MemManager::create_tail(size1, size2, defaultName);
        } else {
            unk0 = MemManager::create(size1, size2, defaultName);
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
