// Decompiled target(s):
//   - us-80463828 Class_8045F858::Class_8045F858(UnkClass_8045F564*)
//   - us-80463898 Class_8045F858::~Class_8045F858()

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util/MemManager.hpp"

using namespace mtl;

Class_8045F858::Class_8045F858(UnkClass_8045F564* ptr) {
    unk0 = ptr;
    
    CLibLayout* layout = CLibLayout::spInstance;
    u32 idx = (layout->hashAccum + layout->hashCount) % layout->hashDivisor;
    layout->hashTable[idx] = ptr;
    layout->hashCount++;
    
    MemManager::setMemInitFlag(false);
}

Class_8045F858::~Class_8045F858() {
    CLibLayout* layout = CLibLayout::spInstance;
    layout->hashCount--;
    
    MemManager::setMemInitFlag(true);
}
