#pragma once

#include <types.h>

namespace cf {

    // ── Opaque struct for func_8009D764 ─────────────────────────────────────
    //   6 u16s (set to 0xFFFF) + 192-byte zeroed area.
    struct CtrlObjectParamInit {
        u16 field_00;
        u16 field_02;
        u16 field_04;
        u16 field_06;
        u16 field_08;
        u16 field_0A;
        u8  blob[192];
    };

    // ── 32-byte sub-struct entry (getSubStruct / getShortAt1C / getByteE4) ──
    struct CtrlObjectParamSubEntry {
        u8  pad_00[0x10];        // 0x00..0x0F
        s16 shortArr[8];         // 0x10..0x1F  - accessed at work+0x1C = entry[0].shortArr
    };

    // ── Opaque data block with sub-entries (getSubStruct, getShortAt1C, getByteE4) ──
    struct CtrlObjectParamData {
        u8                       pad_0C[0x0C];          // 0x00..0x0B
        CtrlObjectParamSubEntry  entries[6];            // 0x0C..0xCB  (6×32=192)
        u8                       pad_CC[0x18];           // 0xCC..0xE3
        u8                       field_E4;               // 0xE4

        CtrlObjectParamSubEntry* getSubStruct(unsigned long index);
        void setArgType2(void* arg);
        void setArgType3(void* arg);
        void setArgType5(void* arg);
        long getShortAt1C(unsigned long index);
    };

    // ── Swap view (swapIntFields): int arrays at +4 and +16 ─────────────────
    struct CtrlObjectParamSwap {
        u8  pad_00[4];       // 0x00..0x03
        int intArr1[3];      // 0x04..0x0F  (type 1, indices 0,1,2)
        int intArr2[3];      // 0x10..0x1B  (type 2, indices 0,1,2)
    };

    // ── Clear view (clearStruct): byte + 24 ints ───────────────────────────
    struct CtrlObjectParamClear {
        u8  firstByte;        // 0x00
        u32 words[24];        // 0x04..0x63

        int clearStruct();
    };

    // ── Clear 16 bytes view (clear16Bytes) ─────────────────────────────────
    struct CtrlObjectParamClear16 {
        u32 words[4];         // 0x00..0x0F

        void clear16Bytes();
    };

    // ── Byte-at-E4 view (getByteE4) ────────────────────────────────────────
    struct CtrlObjectParamByteE4 {
        u8  pad_00[0xE4];
        u8  field_E4;

        u8 getByteE4();
    };

} // namespace cf
