#include "kyoshin/cf/CArtsSet.hpp"

// Records viewed as polymorphic objects: the 0x84-byte non-polymorphic data
// base places the vptr at 0x84 (retail loads the table pointer from there),
// and the per-record init routine is vtable slot 2 (offset 8) - the same
// dispatch shape the retail CAttackSet/CArtsSet init loops emit.
//
// The init virtual is declared PURE: the real init implementations live in
// CArtsParam.cpp (CAttackParam::CAttackParam_UnkVirtualFunc1 and CArtsParam's
// override). This TU only needs the dispatch shape (load the record's table
// pointer at +0x84, jump to slot 2) - the record objects are never
// constructed here, so these class vtables are never dispatched through at
// runtime; the game data carries the real per-record table pointers.
struct CAttackParamData {
    u8 field_0[0x84];
};

struct CAttackParamVtblRec : CAttackParamData {
    virtual void vtInit() = 0;  // first virtual -> vtable slot 2 (offset 8, MWCC 2-slot vtable overhead)
};

// 0x8c-strided arts-param record: vptr at 0x84, 4 bytes of derived data at 0x88.
struct CArtsParamVtblRec : CAttackParamData {
    virtual void vtInit() = 0;  // first virtual -> vtable slot 2 (offset 8)
    u8 field_88[0x8c - 0x88];
};

namespace cf {
    _sArtsSet::_sArtsSet() {
        _sArtsSet_UnkVirtualFunc1();
    }

    void CArtsSet::CArtsSet_UnkVirtualFunc1() {
        unk0 = 0;
        std::memset(unk4, 0, 0x30);

        // Function-scope rowBase/p/row declaration order drives the Chaitin
        // homes to r31/r30/r29, matching the retail init loop.
        CArtsParamVtblRec* rowBase;
        CArtsParamVtblRec* p;
        int row;

        rowBase = reinterpret_cast<CArtsParamVtblRec*>(&mArtsParams[0]);
        for (row = 0; row < 3; row++) {
            p = rowBase;
            for (int col = 0; col < 8; col++) {
                p->vtInit();
                p++;
            }
            rowBase += 8;  // 8 * 0x8c = 0x460 bytes per row
        }
    }

    // Retrieve a 16-bit arts-slot entry at row*0x10 + col*0x2 bytes in.
    // `this`(r3) accumulates both strides, matching the retail rlwinm/mr pair.
    // Accumulate the row*0x10 and col*0x2 strides into a running byte pointer
    // (mirrors func_80153CAC), so MWCC keeps `this`(r3) as the accumulator and
    // reuses each source register for its own shift.
    void CArtsSet::setArtsSlotRC(unsigned short value, unsigned short row, unsigned short index) {
        u8* p = (u8*)this;
        p += row * 0x10;
        p += index * 0x2;
        *(u16*)(p + 0x4) = value;
    }

    unsigned short CArtsSet::getArtsSlotRC(int index, int subindex) {
        u8* p = (u8*)this;
        p += index * 0x10;
        p += subindex * 0x2;
        return *(u16*)(p + 0x4);
    }

    // Slot lookup keyed by the current slot count stored in the first word
    // (mArtsSlotData[0]): the requested slot sits at byte +4 (2 u16 entries
    // past the count word) on the row*0x10 + index*2 grid - the same strides
    // as getArtsSlotRC. Out-of-range index reads past the declared grid,
    // which is intentional (retail performs the same raw memory access).
    unsigned short CArtsSet::getArtsSlotAtCnt(unsigned int index) {
        unsigned short count = mArtsSlotData[0];
        return *reinterpret_cast<unsigned short*>(
            reinterpret_cast<unsigned char*>(this) + 4 + count * 0x10 + index * 0x2);
    }

    // Decompose the flat index into row/col and store into the slot entry.
    void CArtsSet::setArtsSlotByIdx(unsigned short value, int index) {
        u8* p = (u8*)this;
        p += (index / 8) * 0x10;
        p += (index % 8) * 0x2;
        *(u16*)(p + 0x4) = value;
    }

    void* CArtsSet::getArtsParamRC(int index460, int index8c) {
        return &mArtsParams[index460 * 8 + index8c];
    }

    // Return the CArtsParam at row*0x460 + col*0x8c bytes into mArtsParams
    // (0x38 from the CArtsSet base); array indexing keeps the two mulli
    // strides separate and reuses the source registers like the retail build.
    // Accumulate the index1*0x460 and index2*0x8c strides into this(r3), so the
    // two mullis land in the source registers (r4/r0) like the retail build.
    void* CArtsSet::getArtsParamRC2(int index1, int index2) {
        u8* p = (u8*)this;
        p += index1 * 0x460;
        p += index2 * 0x8c;
        return p + 0x38;
    }

    // Decompose the flat index into row/col and return the CArtsParam entry.
    void* CArtsSet::getArtsParamByIdx(int index) {
        u8* p = (u8*)this;
        p += (index / 8) * 0x460;
        p += (index % 8) * 0x8c;
        return p + 0x38;
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

// C-ABI accessor (retail symbol is unmangled): returns the arts-param record
// at the row stamped in the first word (count*0x460) plus the given index
// stride (index*0x8c), 0x38 bytes past the CArtsSet base.
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

// C-ABI accessor (retail symbol is unmangled): returns the attack-param
// record at base + index*0x88 + 0x10.
extern "C" void* getAtkParam(void* base, int index) { return (char*)base + index * 0x88 + 0x10; }

void func_80153E88(void* self) {
    std::memset(self, 0, 0xc);
    CAttackParamVtblRec* arr = reinterpret_cast<CAttackParamVtblRec*>(reinterpret_cast<unsigned char*>(self) + 0x10);
    for (int i = 0; i < 6; i++) {
        arr[i].vtInit();
    }
}
