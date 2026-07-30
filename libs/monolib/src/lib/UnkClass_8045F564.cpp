#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/util/MemManager.hpp"
#include <revolution/mem.h>

using namespace mtl;

UnkClass_8045F564::UnkClass_8045F564()
    : unk0(-1), unk4(0), unk8(0), unkC(0) {}

UnkClass_8045F564::~UnkClass_8045F564() {}

void UnkClass_8045F564::createRegion(int, int, const char*, int) {}

void UnkClass_8045F564::func_8045F778() {
    if (unk0 == -1) return;
    if (unk4 != 0) {
        MEMDestroyFrmHeap(reinterpret_cast<MEMiHeapHead*>(unk4));
        unk4 = 0;
    }
    MemManager::func_80434A4C(false);
    MemManager::erase(unk0);
    MemManager::func_80434A4C(true);
    unk0 = -1;
}

void UnkClass_8045F564::func_8045F7E8() {
    if (unk0 == -1) return;
    if (unk4 == 0) return;
    MEMFreeToFrmHeap(reinterpret_cast<MEMiHeapHead*>(unk4), 3);
}

void UnkClass_8045F564::func_8045F810() {
    MemManager::func_804348A4(unk0, 1);
    if (unk4 != 0) {
        MEMGetAllocatableSizeForFrmHeapEx(reinterpret_cast<MEMiHeapHead*>(unk4), 4);
    }
}
