#include "kyoshin/cf/CArtsSet.hpp"

void* memset(void* dest, int val, size_t count);

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

void setArtsSlotRC(cf::CArtsSet* self, unsigned short value, unsigned int row, unsigned int index) { unsigned char* base = reinterpret_cast<unsigned char*>(self) + ((row & 0xffffu) << 4); base += (index & 0xffffu) << 1; *reinterpret_cast<unsigned short*>(base + 4) = value; }
unsigned short getArtsSlotRC(void* this_, int index, int subindex) { return ((unsigned short*)((char*)this_ + (index << 4) + 4))[subindex]; }
void setArtsSlotByIdx(cf::CArtsSet* self, unsigned short value, int index) {
    ((unsigned short (*)[8])((unsigned char*)self + 4))[index / 8][index % 8] = value;
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
void* getArtsParamRC(cf::CArtsSet* self, int index460, int index8c) { char* result = (char*)self + index460 * 0x460; result += index8c * 0x8c; return result + 0x38; }
extern "C" void* getArtsParamAtCnt(void* self, unsigned int index) { unsigned short count = *static_cast<unsigned short*>(self); return static_cast<unsigned char*>(self) + 0x38 + count * 0x460 + index * 0x8c; }
void func_80153DCC(){}
extern "C" void* getArtsParamRC2(void* this_, int index1, int index2) { char* result = (char*)this_ + index1 * 0x460; result += index2 * 0x8c; return result + 0x38; }
extern "C" void* getArtsParamByIdx(void* self, int index) { return (char*)self + 0x38 + (index / 8) * 0x460 + (index % 8) * 0x8c; }
extern "C" void* getAtkParam(void* base, int index) { return (char*)base + index * 0x88 + 0x10; }

void func_80153E88(){}
