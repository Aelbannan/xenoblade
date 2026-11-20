#pragma once

#include <types.h>

class CfEnumActorAux;

// Real object-enumeration base class used by CfObjectEnumList.cpp's
// CfEnumActor and by callers that receive a findObjectById result. The
// virtual slots are the retail vtable order; the unnamed slots still name
// their vtable index until their retail symbols are recovered.
#define CF_ENUM_VIRTUAL(n_) virtual void v##n_() = 0

class CfEnumObjectBase {
public:
    CF_ENUM_VIRTUAL(0);  CF_ENUM_VIRTUAL(1);  CF_ENUM_VIRTUAL(2);
    CF_ENUM_VIRTUAL(3);  CF_ENUM_VIRTUAL(4);  CF_ENUM_VIRTUAL(5);
    CF_ENUM_VIRTUAL(6);  CF_ENUM_VIRTUAL(7);  CF_ENUM_VIRTUAL(8);
    CF_ENUM_VIRTUAL(9);  CF_ENUM_VIRTUAL(10); CF_ENUM_VIRTUAL(11);
    CF_ENUM_VIRTUAL(12); CF_ENUM_VIRTUAL(13); CF_ENUM_VIRTUAL(14);
    CF_ENUM_VIRTUAL(15); CF_ENUM_VIRTUAL(16); CF_ENUM_VIRTUAL(17);
    CF_ENUM_VIRTUAL(18); CF_ENUM_VIRTUAL(19); CF_ENUM_VIRTUAL(20);
    CF_ENUM_VIRTUAL(21); CF_ENUM_VIRTUAL(22); CF_ENUM_VIRTUAL(23);
    CF_ENUM_VIRTUAL(24); CF_ENUM_VIRTUAL(25); CF_ENUM_VIRTUAL(26);
    virtual bool isEnabled() = 0; // vtable +0x74
    CF_ENUM_VIRTUAL(28); CF_ENUM_VIRTUAL(29); CF_ENUM_VIRTUAL(30);
    CF_ENUM_VIRTUAL(31); CF_ENUM_VIRTUAL(32); CF_ENUM_VIRTUAL(33);
    CF_ENUM_VIRTUAL(34); CF_ENUM_VIRTUAL(35); CF_ENUM_VIRTUAL(36);
    CF_ENUM_VIRTUAL(37); CF_ENUM_VIRTUAL(38); CF_ENUM_VIRTUAL(39);
    CF_ENUM_VIRTUAL(40); CF_ENUM_VIRTUAL(41); CF_ENUM_VIRTUAL(42);
    CF_ENUM_VIRTUAL(43); CF_ENUM_VIRTUAL(44); CF_ENUM_VIRTUAL(45);
    CF_ENUM_VIRTUAL(46); CF_ENUM_VIRTUAL(47); CF_ENUM_VIRTUAL(48);
    CF_ENUM_VIRTUAL(49); CF_ENUM_VIRTUAL(50); CF_ENUM_VIRTUAL(51);
    CF_ENUM_VIRTUAL(52); CF_ENUM_VIRTUAL(53); CF_ENUM_VIRTUAL(54);
    CF_ENUM_VIRTUAL(55); CF_ENUM_VIRTUAL(56); CF_ENUM_VIRTUAL(57);
    CF_ENUM_VIRTUAL(58); CF_ENUM_VIRTUAL(59); CF_ENUM_VIRTUAL(60);
    CF_ENUM_VIRTUAL(61); CF_ENUM_VIRTUAL(62); CF_ENUM_VIRTUAL(63);
    CF_ENUM_VIRTUAL(64); CF_ENUM_VIRTUAL(65); CF_ENUM_VIRTUAL(66);
    CF_ENUM_VIRTUAL(67); CF_ENUM_VIRTUAL(68); CF_ENUM_VIRTUAL(69);
    CF_ENUM_VIRTUAL(70); CF_ENUM_VIRTUAL(71); CF_ENUM_VIRTUAL(72);
    CF_ENUM_VIRTUAL(73); CF_ENUM_VIRTUAL(74); CF_ENUM_VIRTUAL(75);
    CF_ENUM_VIRTUAL(76); CF_ENUM_VIRTUAL(77); CF_ENUM_VIRTUAL(78);
    CF_ENUM_VIRTUAL(79); CF_ENUM_VIRTUAL(80); CF_ENUM_VIRTUAL(81);
    CF_ENUM_VIRTUAL(82); CF_ENUM_VIRTUAL(83); CF_ENUM_VIRTUAL(84);
    CF_ENUM_VIRTUAL(85);
    virtual bool filterA() = 0;  // vtable +0x160
    virtual bool filterB() = 0;  // vtable +0x164
    CF_ENUM_VIRTUAL(88);
    virtual float filterC() = 0; // vtable +0x16C
    CF_ENUM_VIRTUAL(90);
    virtual float filterD() = 0; // vtable +0x174
};

// Data portion of the enumerated object. The +0x91 type byte is the
// actor-kind discriminator read by CMenuKeyAssign's target-selection gate.
class CfEnumObject : public CfEnumObjectBase {
public:
    u8 _pad_004[0x64 - 0x04];
    u32 flags64;
    u32 field68;            // 0x68 (mFlags68)
    u8 _pad_06C[0x74 - 0x6C];
    u32 objectId;
    u8 _pad_078[0x8C - 0x78];
    u16 type8C;
    u8 _pad_08E[0x91 - 0x8E];
    u8 type91;              // 0x91 actor kind
    u8 _pad_092[0xC4 - 0x92];
    CfEnumActorAux* actorAux;
};

#undef CF_ENUM_VIRTUAL
