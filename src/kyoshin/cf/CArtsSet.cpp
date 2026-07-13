#include "kyoshin/cf/CArtsSet.hpp"

extern "C" void* memset(void* dest, int val, size_t count);

namespace cf {
    _sArtsSet::_sArtsSet() {
        _sArtsSet_UnkVirtualFunc1();
    }

    void CArtsSet::CArtsSet_UnkVirtualFunc1() {
        CArtsParam* rowBase;
        CArtsParam* p;
        int row;

        unk0 = 0;
        memset(unk4, 0, 0x30);

        rowBase = (CArtsParam*)((char*)this + 0x38);
        for (row = 0; row < 3; row++) {
            p = rowBase;
            for (int col = 0; col < 8; col++) {
                static_cast<CAttackParam*>(p)->CAttackParam_UnkVirtualFunc1();
                p = (CArtsParam*)((char*)p + 0x8C);
            }
            rowBase = (CArtsParam*)((char*)rowBase + 0x460);
        }
    }
}
