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

        CAttackParam();

        virtual void CAttackParam_UnkVirtualFunc1(){
            unk0 = 0;
            unk20 = 0;
            unk24 = 0;
            unk28 = 0;
            unk2A = 1;
            unk2B = 0;
            unk2C = 0;
            unk30 = 0;
            unk34 = 0;
            unk36 = 0;
            unk3C = 0;
            unk3E = 0;
            unk40 = 0;
            unk42 = 0;
            unk43 = 0;
            unk44 = 0;
            unk46 = 0;
            unk48 = 0;
            unk4A = 0;
            unk4C = 0;
            unk50 = 0;
            unk54 = 0;
            unk58 = 0;
            unk5A = 0;
            unk5C = 0;
            unk5E = 0;
            unk60 = 0;
            unk64 = 0;
            unk66 = 0;
            unk67 = 0;
            unk68 = 0;
            unk6A = 0;
            unk72 = 0;
            unk74 = 0;
            unk77 = 0;
            unk7C = 0;
            unk80 = 0;  

            std::memset(unk38, 0, sizeof(unk38));
            std::memset(unk6C, 0, sizeof(unk6C));
        }
        virtual u8 CAttackParam_UnkVirtualFunc2();
        virtual void CAttackParam_UnkVirtualFunc3(u8 r4);
        virtual void CAttackParam_UnkVirtualFunc4();
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
        virtual void CArtsParam_UnkVirtualFunc1();
        virtual u8 CArtsParam_UnkVirtualFunc2();
        virtual void CArtsParam_UnkVirtualFunc3(u8 r4);
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
        virtual void _sArtsSet_UnkVirtualFunc1(){
            unk0 = 0;
            std::memset(unk4, 0, sizeof(unk4));
        }
    };

    //size: 0xD58
    class CArtsSet : _sArtsSet {
    public:
        CArtsSet(){}
        virtual void CArtsSet_UnkVirtualFunc1();

        void setArtsSlotRC(unsigned short value, unsigned int row, unsigned int index);
        unsigned short getArtsSlotRC(int index, int subindex);
        void setArtsSlotByIdx(unsigned short value, int index);
        void* getArtsParamRC(int index460, int index8c);
        void* getArtsParamRC2(int index1, int index2);
        void* getArtsParamByIdx(int index);

        //0x0: vtable
        //0x0-38: _sArtsSet
        CArtsParam mArtsParams[24]; //0x38
    };
};
