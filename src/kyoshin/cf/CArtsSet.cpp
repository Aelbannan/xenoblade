#include "kyoshin/cf/CArtsSet.hpp"

void* memset(void* dest, int val, size_t count);

namespace cf {
    _sArtsSet::_sArtsSet() {
        _sArtsSet_UnkVirtualFunc1();
    }

    void CArtsSet::CArtsSet_UnkVirtualFunc1() {
        unk0 = 0;
        std::memset(unk4, 0, 0x30);

        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 8; col++) {
                mArtsParams[row * 8 + col].CAttackParam_UnkVirtualFunc1();
            }
        }
    }

    void CArtsSet::setArtsSlotRC(unsigned short value, unsigned int row, unsigned int index) {
        mArtsSlotData[row * 8 + index] = value;
    }

    unsigned short CArtsSet::getArtsSlotRC(int index, int subindex) {
        return mArtsSlotData[index * 8 + subindex];
    }

    void CArtsSet::setArtsSlotByIdx(unsigned short value, int index) {
        mArtsSlotData[index] = value;
    }

    void* CArtsSet::getArtsParamRC(int index460, int index8c) {
        return &mArtsParams[index460 * 8 + index8c];
    }

    void* CArtsSet::getArtsParamRC2(int index1, int index2) {
        return &mArtsParams[index1 * 8 + index2];
    }

    void* CArtsSet::getArtsParamByIdx(int index) {
        return &mArtsParams[index];
    }
}

void func_80153CAC(){}

unsigned short getArtsSlotAtCnt(const void* this_, unsigned int index) {
    const unsigned char* base = static_cast<const unsigned char*>(this_);
    unsigned short count = *reinterpret_cast<const unsigned short*>(base);
    unsigned int indexOffset = index << 1;
    unsigned int countOffset = static_cast<unsigned int>(count) << 4;
    const unsigned char* record = base + countOffset;
    record += indexOffset;
    return *reinterpret_cast<const unsigned short*>(record + 4);
}

extern "C" void* getArtsParamAtCnt(void* self, unsigned int index) { unsigned short count = *static_cast<unsigned short*>(self); return static_cast<unsigned char*>(self) + 0x38 + count * 0x460 + index * 0x8c; }
void func_80153DCC(){}
extern "C" void* getAtkParam(void* base, int index) { return (char*)base + index * 0x88 + 0x10; }

void func_80153E88(){}
