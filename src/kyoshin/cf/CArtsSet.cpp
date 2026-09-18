#include "kyoshin/cf/CArtsSet.hpp"

// Per-record init is virtual slot +0x08 on the record itself (vptr at
// +0x84; CAttackParam table lbl_eu_8052F610, CArtsParam table
// lbl_eu_8052F5E8). Call sites dispatch through the real base virtual, so
// MWCC emits the retail shape (lwz r12, 0x84(rX) / lwz r12, 0x8(r12) /
// bctrl) with no reinterpret-cast views.

// Note on function order: the object emits functions in source order, and the
// retail .o lists them in this exact order (ctor, the sArtsSet init virtual,
// slot accessors, free helpers, the CArtsSet init virtual, param accessors,
// then the tail helpers).

namespace cf {

    _sArtsSet::_sArtsSet() {
        _sArtsSet_UnkVirtualFunc1();
    }

    // The virtual's body, defined under the retail mangled name so no local
    // __vt__/RTTI is emitted (retail CArtsSet.o is text-only; the vtables
    // live in the shared data split, lbl_eu_8052F5D8).
    extern "C" void _sArtsSet_UnkVirtualFunc1__Q22cf9_sArtsSetFv(_sArtsSet* self) {
        self->unk0 = 0;
        std::memset(self->unk4, 0, 0x30);
    }

    // Retrieve a 16-bit arts-slot entry at row*0x10 + col*0x2 bytes in.
    // `this`(r3) accumulates both strides, matching the retail rlwinm/mr pair.
    // Accumulate the row*0x10 and col*0x2 strides into a running byte pointer
    // (mirrors getArtsSlotByFlatIdx), so MWCC keeps `this`(r3) as the accumulator and
    // reuses each source register for its own shift.
    void CArtsSet::setArtsSlotRC(unsigned short value, unsigned short row, unsigned short index) {
        // Match-pinned (u8*)this strides (row*0x10 + col*2 + 4).
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

    // Decompose the flat index into row/col and store into the slot entry.
    void CArtsSet::setArtsSlotByIdx(unsigned short value, int index) {
        u8* p = (u8*)this;
        p += (index / 8) * 0x10;
        p += (index % 8) * 0x2;
        *(u16*)(p + 0x4) = value;
    }
}

unsigned short getArtsSlotByFlatIdx(const cf::CArtsSet* set, int index) {
    int row = index / 8;
    int col = index % 8;
    const char* p = reinterpret_cast<const char*>(set);
    p += row * 0x10;
    p += col * 2;
    return *reinterpret_cast<const unsigned short*>(p + 4);
}

namespace cf {
    // Slot lookup keyed by the current slot count stored in the first word
    // (mArtsSlotData[0]): the requested slot sits at byte +4 (2 u16 entries
    // past the count word) on the row*0x10 + index*2 grid - the same strides
    // as getArtsSlotRC. Out-of-range index reads past the declared grid,
    // which is intentional (retail performs the same raw memory access).
    unsigned short CArtsSet::getArtsSlotAtCnt(unsigned int index) {
        unsigned short count = mArtsSlotData[0];
        // Match-pinned: count*0x10 + index*2 at +4 (same grid as getArtsSlotRC).
        u8* p = (u8*)this;
        p += count * 0x10;
        p += index * 0x2;
        return *(u16*)(p + 0x4);
    }

    // The CArtsSet init virtual's body, defined under the retail mangled name
    // (see the _sArtsSet_UnkVirtualFunc1 note above).
    extern "C" void CArtsSet_UnkVirtualFunc1__Q22cf8CArtsSetFv(CArtsSet* self) {
        // Public _sArtsSet base: same typed clears as _sArtsSet_UnkVirtualFunc1.
        self->unk0 = 0;
        std::memset(self->unk4, 0, 0x30);

        // Function-scope rowBase/p/row declaration order drives the Chaitin
        // homes to r31/r30/r29, matching the retail init loop.
        CArtsParam* rowBase;
        CArtsParam* p;
        int row;

        rowBase = &self->mArtsParams[0];
        for (row = 0; row < 3; row++) {
            p = rowBase;
            for (int col = 0; col < 8; col++) {
                p->CAttackParam_clearArtsRecord();
                p++;
            }
            rowBase += 8;  // 8 * 0x8c = 0x460 bytes per row
        }
    }

    CArtsParam* CArtsSet::getArtsParamRC(int index460, int index8c) {
        // Explicit strides: `&mArtsParams[index460 * 8 + index8c]` folds the
        // x8 into the 0x8c mulli (rlwinm + mulli); the explicit 0x460/0x8c
        // strides emit the retail pair of mullis.
        u8* p = (u8*)this;
        p += index460 * 0x460;
        p += index8c * 0x8c;
        return reinterpret_cast<CArtsParam*>(p + 0x38);
    }
}

// C-ABI accessor (retail symbol is unmangled): returns the arts-param record
// at the row stamped in the first word (count*0x460) plus the given index
// stride (index*0x8c), 0x38 bytes past the CArtsSet base.
extern "C" cf::CArtsParam* getArtsParamAtCnt(cf::CArtsSet* set, unsigned int index) {
    unsigned short count = set->mArtsSlotData[0];
    return reinterpret_cast<cf::CArtsParam*>(
        reinterpret_cast<unsigned char*>(set) + 0x38 + count * 0x460 + index * 0x8c);
}

extern cf::CArtsParam lbl_eu_80573D88;

cf::CArtsParam* lookupArtsParamById(cf::CArtsSet* self, int id) {
    unsigned char* outer = reinterpret_cast<unsigned char*>(self);
    unsigned char* inner;
    int row = 0;
    do {
        inner = outer;
        for (int col = 0; col < 8; col++) {
            // The CArtsParam record starts 0x38 bytes in; unk77/unk20 land
            // on the retail 0xaf/0x58 loads after offset folding.
            cf::CArtsParam* rec = reinterpret_cast<cf::CArtsParam*>(inner + 0x38);
            if (id == rec->unk77 &&
                rec->unk20 != 0) {
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

namespace cf {
    // Return the CArtsParam at row*0x460 + col*0x8c bytes into mArtsParams
    // (0x38 from the CArtsSet base); array indexing keeps the two mulli
    // strides separate and reuses the source registers like the retail build.
    // Accumulate the index1*0x460 and index2*0x8c strides into this(r3), so the
    // two mullis land in the source registers (r4/r0) like the retail build.
    CArtsParam* CArtsSet::getArtsParamRC2(int index1, int index2) {
        u8* p = (u8*)this;
        p += index1 * 0x460;
        p += index2 * 0x8c;
        return reinterpret_cast<CArtsParam*>(p + 0x38);
    }

    // Decompose the flat index into row/col and return the CArtsParam entry.
    CArtsParam* CArtsSet::getArtsParamByIdx(int index) {
        u8* p = (u8*)this;
        p += (index / 8) * 0x460;
        p += (index % 8) * 0x8c;
        return reinterpret_cast<CArtsParam*>(p + 0x38);
    }
}

void resetAttackSetArts(cf::CAttackSet* set) {
    std::memset(set, 0, 0xc);
    cf::CAttackParam* arr = reinterpret_cast<cf::CAttackParam*>(reinterpret_cast<unsigned char*>(set) + 0x10);
    for (int i = 0; i < 6; i++) {
        arr[i].CAttackParam_clearArtsRecord();
    }
}

// C-ABI accessor (retail symbol is unmangled): returns the attack-param
// record at base + index*0x88 + 0x10.
extern "C" cf::CAttackParam* getAtkParam(cf::CAttackSet* set, int index) {
    return reinterpret_cast<cf::CAttackParam*>(
        reinterpret_cast<char*>(set) + index * 0x88 + 0x10);
}
