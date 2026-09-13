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
        virtual void CAttackParam_clearArtsRecord();      // +0x08
        // +0x0C returns full-word: both retail callers use the bctrl result
        // with no rlwinm extend (CtrlAct.s getArtsGaugeMax subi/mullw direct;
        // CActorParam.s xoris direct), so the decl is int, not u8.
        // Same slot the CArtsParam override names getArtsFlagByte
        // (retail lbz 0x2A base impl here).
        virtual int CAttackParam_getArtsFlagByte();       // +0x0C
        virtual void CAttackParam_UnkVirtualFunc3(u8 r4); // +0x10
        virtual float CAttackParam_getArtsGaugeMax();     // +0x14

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
    // Retail vtable lbl_eu_8052F5E8: overrides slots +0x08/+0x0C/+0x10,
    // inherits +0x14. The overrides are declared non-virtual under the
    // novtable convention (same as CHelpSwitch::func_802B7CB0): making them
    // virtual emits a compiler __vt__ the retail TU never had, so the ctor
    // installs the retail label through vtbl() instead.
    class CArtsParam : public CAttackParam {
    public:
        UNKTYPE* unk88;

        CArtsParam();
        void CArtsParam_clearArtsRecord();   // +0x08 override (base clear + unk88 = 0)
        u8 CArtsParam_getArtsFlagByte();     // +0x0C override (*unk88 byte, else unk2A)
        void vtableFunc3(u8 val);            // +0x10 override (*unk88 byte = val)
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
