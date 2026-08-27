#pragma once

#include <types.h>
#include <cstring>

namespace cf {

    //size: 0x88
    class CAttackParam {
    public:
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
        void* unk84;   // 0x84 - raw vtable (slot 3 = +0x0C hook, read by UnkVirtualFunc4)

        CAttackParam();

        void CAttackParam_UnkVirtualFunc1();
        void CAttackParam_UnkVirtualFunc2();
        void CAttackParam_UnkVirtualFunc3(u8 r4);
        void CAttackParam_UnkVirtualFunc4();
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
