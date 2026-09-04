#pragma once

#include <types.h>
#include <cstring>

namespace cf {

    // 0x84 bytes of non-polymorphic record data. The C++ vptr follows at
    // +0x84 (MWCC lays the derived vptr after a non-polymorphic prefix base;
    // same scheme as CHelp/CHelpPrefix with vptr at +8). Retail stores the
    // record tables there (CAttackParam: lbl_eu_8052F610, CArtsParam:
    // lbl_eu_8052F5E8) and dispatches slot +0x08 through them:
    //   lwz r12, 0x84(rX) / lwz r12, 0x8(r12) / bctrl
    struct CAttackParamPrefix {
        u8 unk0;
        u8 unk4[0x20 - 0x4];
        u32 unk20;
        u32 unk24;
        u16 unk28;
        u8 unk2A;
        u8 unk2B;
        float unk2C;
        float unk30;
        u16 unk34;
        u16 unk36;
        u8 unk38[4];
        u16 unk3C;
        u8 unk3E;
        u8 unk3F;
        u16 unk40;
        u8 unk42;
        u8 unk43;
        u8 unk44;
        u16 unk46;
        u16 unk48;
        u16 unk4A;
        u16 unk4C;
        float unk50;
        float unk54;
        u16 unk58;
        u16 unk5A;
        u16 unk5C;
        u16 unk5E;
        float unk60;
        u16 unk64;
        u8 unk66;
        u8 unk67;
        u16 unk68;
        u16 unk6A;
        u8 unk6C[5];
        u8 unk71;
        u16 unk72;
        u16 unk74;
        u8 unk76;
        u8 unk77;
        u32 unk78;
        float unk7C;
        float unk80;
    };

    //size: 0x88 (0x84 data + vptr at +0x84)
    // Retail vtable lbl_eu_8052F610 (RTTI lbl_eu_80662280): four virtuals.
    // novtable: the tables live in the shared data split, so this TU never
    // emits __vt__; the ctor installs the retail label through vtbl().
    class __declspec(novtable) CAttackParam : public CAttackParamPrefix {
    public:
        virtual void CAttackParam_UnkVirtualFunc1();      // +0x08
        virtual u8 CAttackParam_UnkVirtualFunc2();        // +0x0C
        virtual void CAttackParam_UnkVirtualFunc3(u8 r4); // +0x10
        virtual float CAttackParam_UnkVirtualFunc4();     // +0x14

        CAttackParam();

        // Overlay on the vptr at +0x84 so the ctor can install the retail
        // table (same pattern as CHelp::vtbl at +8).
        void*& vtbl() {
            return *reinterpret_cast<void**>(reinterpret_cast<u8*>(this) + 0x84);
        }
    };

    struct _sAttackSet {
    };

    //size: 0x334
    class CAttackSet : _sAttackSet {
    public:
        CAttackSet(){}
        virtual void func_80153E88();
        
        //0x0: vtable
        CAttackParam mAttackParams[6]; //0x4
    };

    //size: 0x8C
    class CArtsParam : public CAttackParam {
    public:
        UNKTYPE* unk88;

        CArtsParam();
        void CArtsParam_UnkVirtualFunc1();
        u8 CArtsParam_UnkVirtualFunc2();
        void CArtsParam_UnkVirtualFunc3(u8 r4);
        void vtableFunc3(u8 val);
    };

    //size: 0x38
    struct _sArtsSet {
        union {
            struct {
                u16 unk0;
                u8 unk2[2];
                u8 unk4[0x30];
            };
            u16 mArtsSlotData[24];
        };

        _sArtsSet();
        virtual void _sArtsSet_UnkVirtualFunc1();
    };

    //size: 0xD58
    class CArtsSet : _sArtsSet {
    public:
        CArtsSet(){}
        virtual void CArtsSet_UnkVirtualFunc1();

        void setArtsSlotRC(unsigned short value, unsigned short row, unsigned short index);
        unsigned short getArtsSlotRC(int index, int subindex);
        unsigned short getArtsSlotAtCnt(unsigned int index);
        void setArtsSlotByIdx(unsigned short value, int index);
        void* getArtsParamRC(int index460, int index8c);
        void* getArtsParamRC2(int index1, int index2);
        void* getArtsParamByIdx(int index);

        //0x0: vtable
        //0x0-38: _sArtsSet
        CArtsParam mArtsParams[24]; //0x38
    };
};
