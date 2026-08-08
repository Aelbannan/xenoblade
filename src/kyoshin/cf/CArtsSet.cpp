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

// Record viewed as a 0x8c-strided element within CArtsSet where the
// CArtsParam begins 0x38 bytes in (relative to the CArtsSet base).
// Field offsets below are relative to the record origin, matching the
// retail loads of 0xaf (unk77 + 0x38) and 0x58 (unk20 + 0x38).
struct CArtsRecord {
    u8 field_0[0x58];
    u32 field58;  // 0x58  (unk20 of the nested CArtsParam)
    u8 field_5c[0xaf - 0x5c];
    u8 fieldAF;   // 0xaf  (unk77 of the nested CArtsParam)
};

extern cf::CArtsParam lbl_eu_80573D88;

unsigned short func_80153CAC(const void* self, int index) {
    int row = index / 8;
    int col = index % 8;
    const char* p = static_cast<const char*>(self);
    p += row * 0x10;
    p += col * 2;
    return *reinterpret_cast<const unsigned short*>(p + 4);
}

extern "C" void* getArtsParamAtCnt(void* self, unsigned int index) { unsigned short count = *static_cast<unsigned short*>(self); return static_cast<unsigned char*>(self) + 0x38 + count * 0x460 + index * 0x8c; }

cf::CArtsParam* func_80153DCC(cf::CArtsSet* self, int id) {
    unsigned char* outer = reinterpret_cast<unsigned char*>(self);
    unsigned char* inner;
    int row = 0;
    do {
        inner = outer;
        for (int col = 0; col < 8; col++) {
            if (id == reinterpret_cast<CArtsRecord*>(inner)->fieldAF &&
                reinterpret_cast<CArtsRecord*>(inner)->field58 != 0) {
                unsigned char* rp = reinterpret_cast<unsigned char*>(self);
                rp += row * 0x460;
                rp += col * 0x8c;
                return reinterpret_cast<cf::CArtsParam*>(rp + 0x38);
            }
            inner += 0x8c;
        }
        row++;
        outer += 0x460;
    } while (row < 3);
    return &lbl_eu_80573D88;
}

extern "C" void* getAtkParam(void* base, int index) { return (char*)base + index * 0x88 + 0x10; }

// Function-pointer table for the per-CAttackParam virtual slot used when
// constructing the six attack-param members. vtable slot +2 is invoked.
struct CFunc88Rec {
    u8 field_0[0x84];
    void (**vtbl)(void*);  // 0x84: pointer to a function-pointer table
};

void func_80153E88(void* self) {
    memset(self, 0, 0xc);
    CFunc88Rec* arr = reinterpret_cast<CFunc88Rec*>(reinterpret_cast<unsigned char*>(self) + 0x10);
    for (int i = 0; i < 6; i++) {
        (*(arr[i].vtbl + 2))(&arr[i]);
    }
}
