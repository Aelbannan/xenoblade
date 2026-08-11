// Auto-scaffolded catalog TU for kyoshin/cf/CfObjectEnumList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CfObjectEnumList.hpp"

namespace cf {
    class CfObjectSelectorObj {
    public:
        virtual ~CfObjectSelectorObj();
    };

    void* CfObjEnumList::func_800F6E98(int index) {
        return reinterpret_cast<void*>(mPtrArray[index]->objectId);
    }

    void* CfObjEnumList::func_800F6EAC(unsigned long idx) {
        return mPtrArray[idx]->object;
    }

    void* CfObjEnumList::func_800F6EC0(int index) {
        return mPtrArray[index];
    }
}

void __ct__cf_CfObjEnumList(){}

void func_800F49F8(){}

void func_800F4A98(){}

#define ENUM_DUMMY_VIRTUAL(n_) virtual void v##n_() = 0

class CfEnumObjectBase {
public:
    ENUM_DUMMY_VIRTUAL(0);  ENUM_DUMMY_VIRTUAL(1);  ENUM_DUMMY_VIRTUAL(2);
    ENUM_DUMMY_VIRTUAL(3);  ENUM_DUMMY_VIRTUAL(4);  ENUM_DUMMY_VIRTUAL(5);
    ENUM_DUMMY_VIRTUAL(6);  ENUM_DUMMY_VIRTUAL(7);  ENUM_DUMMY_VIRTUAL(8);
    ENUM_DUMMY_VIRTUAL(9);  ENUM_DUMMY_VIRTUAL(10); ENUM_DUMMY_VIRTUAL(11);
    ENUM_DUMMY_VIRTUAL(12); ENUM_DUMMY_VIRTUAL(13); ENUM_DUMMY_VIRTUAL(14);
    ENUM_DUMMY_VIRTUAL(15); ENUM_DUMMY_VIRTUAL(16); ENUM_DUMMY_VIRTUAL(17);
    ENUM_DUMMY_VIRTUAL(18); ENUM_DUMMY_VIRTUAL(19); ENUM_DUMMY_VIRTUAL(20);
    ENUM_DUMMY_VIRTUAL(21); ENUM_DUMMY_VIRTUAL(22); ENUM_DUMMY_VIRTUAL(23);
    ENUM_DUMMY_VIRTUAL(24); ENUM_DUMMY_VIRTUAL(25); ENUM_DUMMY_VIRTUAL(26);
    virtual bool isEnabled() = 0; // vtable +0x74
    ENUM_DUMMY_VIRTUAL(28); ENUM_DUMMY_VIRTUAL(29); ENUM_DUMMY_VIRTUAL(30);
    ENUM_DUMMY_VIRTUAL(31); ENUM_DUMMY_VIRTUAL(32); ENUM_DUMMY_VIRTUAL(33);
    ENUM_DUMMY_VIRTUAL(34); ENUM_DUMMY_VIRTUAL(35); ENUM_DUMMY_VIRTUAL(36);
    ENUM_DUMMY_VIRTUAL(37); ENUM_DUMMY_VIRTUAL(38); ENUM_DUMMY_VIRTUAL(39);
    ENUM_DUMMY_VIRTUAL(40); ENUM_DUMMY_VIRTUAL(41); ENUM_DUMMY_VIRTUAL(42);
    ENUM_DUMMY_VIRTUAL(43); ENUM_DUMMY_VIRTUAL(44); ENUM_DUMMY_VIRTUAL(45);
    ENUM_DUMMY_VIRTUAL(46); ENUM_DUMMY_VIRTUAL(47); ENUM_DUMMY_VIRTUAL(48);
    ENUM_DUMMY_VIRTUAL(49); ENUM_DUMMY_VIRTUAL(50); ENUM_DUMMY_VIRTUAL(51);
    ENUM_DUMMY_VIRTUAL(52); ENUM_DUMMY_VIRTUAL(53); ENUM_DUMMY_VIRTUAL(54);
    ENUM_DUMMY_VIRTUAL(55); ENUM_DUMMY_VIRTUAL(56); ENUM_DUMMY_VIRTUAL(57);
    ENUM_DUMMY_VIRTUAL(58); ENUM_DUMMY_VIRTUAL(59); ENUM_DUMMY_VIRTUAL(60);
    ENUM_DUMMY_VIRTUAL(61); ENUM_DUMMY_VIRTUAL(62); ENUM_DUMMY_VIRTUAL(63);
    ENUM_DUMMY_VIRTUAL(64); ENUM_DUMMY_VIRTUAL(65); ENUM_DUMMY_VIRTUAL(66);
    ENUM_DUMMY_VIRTUAL(67); ENUM_DUMMY_VIRTUAL(68); ENUM_DUMMY_VIRTUAL(69);
    ENUM_DUMMY_VIRTUAL(70); ENUM_DUMMY_VIRTUAL(71); ENUM_DUMMY_VIRTUAL(72);
    ENUM_DUMMY_VIRTUAL(73); ENUM_DUMMY_VIRTUAL(74); ENUM_DUMMY_VIRTUAL(75);
    ENUM_DUMMY_VIRTUAL(76); ENUM_DUMMY_VIRTUAL(77); ENUM_DUMMY_VIRTUAL(78);
    ENUM_DUMMY_VIRTUAL(79); ENUM_DUMMY_VIRTUAL(80); ENUM_DUMMY_VIRTUAL(81);
    ENUM_DUMMY_VIRTUAL(82); ENUM_DUMMY_VIRTUAL(83); ENUM_DUMMY_VIRTUAL(84);
    ENUM_DUMMY_VIRTUAL(85);
    virtual bool filterA() = 0;  // vtable +0x160
    virtual bool filterB() = 0;  // vtable +0x164
    ENUM_DUMMY_VIRTUAL(88);
    virtual float filterC() = 0; // vtable +0x16C
    ENUM_DUMMY_VIRTUAL(90);
    virtual float filterD() = 0; // vtable +0x174
};

class CfEnumActorAuxBase {
public:
    ENUM_DUMMY_VIRTUAL(0);  ENUM_DUMMY_VIRTUAL(1);  ENUM_DUMMY_VIRTUAL(2);
    ENUM_DUMMY_VIRTUAL(3);  ENUM_DUMMY_VIRTUAL(4);  ENUM_DUMMY_VIRTUAL(5);
    ENUM_DUMMY_VIRTUAL(6);  ENUM_DUMMY_VIRTUAL(7);  ENUM_DUMMY_VIRTUAL(8);
    ENUM_DUMMY_VIRTUAL(9);  ENUM_DUMMY_VIRTUAL(10); ENUM_DUMMY_VIRTUAL(11);
    ENUM_DUMMY_VIRTUAL(12); ENUM_DUMMY_VIRTUAL(13); ENUM_DUMMY_VIRTUAL(14);
    ENUM_DUMMY_VIRTUAL(15); ENUM_DUMMY_VIRTUAL(16); ENUM_DUMMY_VIRTUAL(17);
    ENUM_DUMMY_VIRTUAL(18); ENUM_DUMMY_VIRTUAL(19); ENUM_DUMMY_VIRTUAL(20);
    ENUM_DUMMY_VIRTUAL(21); ENUM_DUMMY_VIRTUAL(22); ENUM_DUMMY_VIRTUAL(23);
    ENUM_DUMMY_VIRTUAL(24); ENUM_DUMMY_VIRTUAL(25); ENUM_DUMMY_VIRTUAL(26);
    ENUM_DUMMY_VIRTUAL(27); ENUM_DUMMY_VIRTUAL(28); ENUM_DUMMY_VIRTUAL(29);
};

class CfEnumActorAux : public CfEnumActorAuxBase {
public:
    virtual bool test(int) = 0; // vtable +0x80
};

class CfEnumObject : public CfEnumObjectBase {
public:
    u8 _pad_004[0x64 - 0x04];
    u32 flags64;
    u8 _pad_068[0x74 - 0x68];
    u32 objectId;
    u8 _pad_078[0x8C - 0x78];
    u16 type8C;
    u8 _pad_08E[0xC4 - 0x8E];
    CfEnumActorAux* actorAux;
};

class CfEnumActorBase {
public:
    ENUM_DUMMY_VIRTUAL(0);   ENUM_DUMMY_VIRTUAL(1);   ENUM_DUMMY_VIRTUAL(2);
    ENUM_DUMMY_VIRTUAL(3);   ENUM_DUMMY_VIRTUAL(4);   ENUM_DUMMY_VIRTUAL(5);
    ENUM_DUMMY_VIRTUAL(6);   ENUM_DUMMY_VIRTUAL(7);   ENUM_DUMMY_VIRTUAL(8);
    ENUM_DUMMY_VIRTUAL(9);   ENUM_DUMMY_VIRTUAL(10);  ENUM_DUMMY_VIRTUAL(11);
    ENUM_DUMMY_VIRTUAL(12);  ENUM_DUMMY_VIRTUAL(13);  ENUM_DUMMY_VIRTUAL(14);
    ENUM_DUMMY_VIRTUAL(15);  ENUM_DUMMY_VIRTUAL(16);  ENUM_DUMMY_VIRTUAL(17);
    ENUM_DUMMY_VIRTUAL(18);  ENUM_DUMMY_VIRTUAL(19);  ENUM_DUMMY_VIRTUAL(20);
    ENUM_DUMMY_VIRTUAL(21);  ENUM_DUMMY_VIRTUAL(22);  ENUM_DUMMY_VIRTUAL(23);
    ENUM_DUMMY_VIRTUAL(24);  ENUM_DUMMY_VIRTUAL(25);  ENUM_DUMMY_VIRTUAL(26);
    ENUM_DUMMY_VIRTUAL(27);  ENUM_DUMMY_VIRTUAL(28);  ENUM_DUMMY_VIRTUAL(29);
    ENUM_DUMMY_VIRTUAL(30);  ENUM_DUMMY_VIRTUAL(31);  ENUM_DUMMY_VIRTUAL(32);
    ENUM_DUMMY_VIRTUAL(33);  ENUM_DUMMY_VIRTUAL(34);  ENUM_DUMMY_VIRTUAL(35);
    ENUM_DUMMY_VIRTUAL(36);  ENUM_DUMMY_VIRTUAL(37);  ENUM_DUMMY_VIRTUAL(38);
    ENUM_DUMMY_VIRTUAL(39);  ENUM_DUMMY_VIRTUAL(40);  ENUM_DUMMY_VIRTUAL(41);
    ENUM_DUMMY_VIRTUAL(42);  ENUM_DUMMY_VIRTUAL(43);  ENUM_DUMMY_VIRTUAL(44);
    ENUM_DUMMY_VIRTUAL(45);  ENUM_DUMMY_VIRTUAL(46);  ENUM_DUMMY_VIRTUAL(47);
    ENUM_DUMMY_VIRTUAL(48);  ENUM_DUMMY_VIRTUAL(49);  ENUM_DUMMY_VIRTUAL(50);
    ENUM_DUMMY_VIRTUAL(51);  ENUM_DUMMY_VIRTUAL(52);  ENUM_DUMMY_VIRTUAL(53);
    ENUM_DUMMY_VIRTUAL(54);  ENUM_DUMMY_VIRTUAL(55);  ENUM_DUMMY_VIRTUAL(56);
    ENUM_DUMMY_VIRTUAL(57);  ENUM_DUMMY_VIRTUAL(58);  ENUM_DUMMY_VIRTUAL(59);
    ENUM_DUMMY_VIRTUAL(60);  ENUM_DUMMY_VIRTUAL(61);  ENUM_DUMMY_VIRTUAL(62);
    ENUM_DUMMY_VIRTUAL(63);  ENUM_DUMMY_VIRTUAL(64);  ENUM_DUMMY_VIRTUAL(65);
    ENUM_DUMMY_VIRTUAL(66);  ENUM_DUMMY_VIRTUAL(67);  ENUM_DUMMY_VIRTUAL(68);
    ENUM_DUMMY_VIRTUAL(69);  ENUM_DUMMY_VIRTUAL(70);  ENUM_DUMMY_VIRTUAL(71);
    ENUM_DUMMY_VIRTUAL(72);  ENUM_DUMMY_VIRTUAL(73);  ENUM_DUMMY_VIRTUAL(74);
    ENUM_DUMMY_VIRTUAL(75);  ENUM_DUMMY_VIRTUAL(76);  ENUM_DUMMY_VIRTUAL(77);
    ENUM_DUMMY_VIRTUAL(78);  ENUM_DUMMY_VIRTUAL(79);  ENUM_DUMMY_VIRTUAL(80);
    ENUM_DUMMY_VIRTUAL(81);  ENUM_DUMMY_VIRTUAL(82);  ENUM_DUMMY_VIRTUAL(83);
    ENUM_DUMMY_VIRTUAL(84);  ENUM_DUMMY_VIRTUAL(85);  ENUM_DUMMY_VIRTUAL(86);
    ENUM_DUMMY_VIRTUAL(87);  ENUM_DUMMY_VIRTUAL(88);  ENUM_DUMMY_VIRTUAL(89);
    ENUM_DUMMY_VIRTUAL(90);  ENUM_DUMMY_VIRTUAL(91);  ENUM_DUMMY_VIRTUAL(92);
    ENUM_DUMMY_VIRTUAL(93);  ENUM_DUMMY_VIRTUAL(94);  ENUM_DUMMY_VIRTUAL(95);
    ENUM_DUMMY_VIRTUAL(96);  ENUM_DUMMY_VIRTUAL(97);  ENUM_DUMMY_VIRTUAL(98);
    ENUM_DUMMY_VIRTUAL(99);  ENUM_DUMMY_VIRTUAL(100); ENUM_DUMMY_VIRTUAL(101);
    ENUM_DUMMY_VIRTUAL(102); ENUM_DUMMY_VIRTUAL(103); ENUM_DUMMY_VIRTUAL(104);
    ENUM_DUMMY_VIRTUAL(105); ENUM_DUMMY_VIRTUAL(106); ENUM_DUMMY_VIRTUAL(107);
    ENUM_DUMMY_VIRTUAL(108); ENUM_DUMMY_VIRTUAL(109); ENUM_DUMMY_VIRTUAL(110);
    ENUM_DUMMY_VIRTUAL(111); ENUM_DUMMY_VIRTUAL(112); ENUM_DUMMY_VIRTUAL(113);
    ENUM_DUMMY_VIRTUAL(114); ENUM_DUMMY_VIRTUAL(115); ENUM_DUMMY_VIRTUAL(116);
    ENUM_DUMMY_VIRTUAL(117); ENUM_DUMMY_VIRTUAL(118); ENUM_DUMMY_VIRTUAL(119);
    ENUM_DUMMY_VIRTUAL(120); ENUM_DUMMY_VIRTUAL(121); ENUM_DUMMY_VIRTUAL(122);
    ENUM_DUMMY_VIRTUAL(123); ENUM_DUMMY_VIRTUAL(124); ENUM_DUMMY_VIRTUAL(125);
    ENUM_DUMMY_VIRTUAL(126); ENUM_DUMMY_VIRTUAL(127); ENUM_DUMMY_VIRTUAL(128);
    ENUM_DUMMY_VIRTUAL(129); ENUM_DUMMY_VIRTUAL(130); ENUM_DUMMY_VIRTUAL(131);
    ENUM_DUMMY_VIRTUAL(132); ENUM_DUMMY_VIRTUAL(133); ENUM_DUMMY_VIRTUAL(134);
    ENUM_DUMMY_VIRTUAL(135); ENUM_DUMMY_VIRTUAL(136); ENUM_DUMMY_VIRTUAL(137);
    ENUM_DUMMY_VIRTUAL(138); ENUM_DUMMY_VIRTUAL(139); ENUM_DUMMY_VIRTUAL(140);
    ENUM_DUMMY_VIRTUAL(141); ENUM_DUMMY_VIRTUAL(142); ENUM_DUMMY_VIRTUAL(143);
    ENUM_DUMMY_VIRTUAL(144); ENUM_DUMMY_VIRTUAL(145); ENUM_DUMMY_VIRTUAL(146);
    ENUM_DUMMY_VIRTUAL(147); ENUM_DUMMY_VIRTUAL(148); ENUM_DUMMY_VIRTUAL(149);
    ENUM_DUMMY_VIRTUAL(150); ENUM_DUMMY_VIRTUAL(151); ENUM_DUMMY_VIRTUAL(152);
    ENUM_DUMMY_VIRTUAL(153); ENUM_DUMMY_VIRTUAL(154); ENUM_DUMMY_VIRTUAL(155);
    ENUM_DUMMY_VIRTUAL(156); ENUM_DUMMY_VIRTUAL(157); ENUM_DUMMY_VIRTUAL(158);
    ENUM_DUMMY_VIRTUAL(159); ENUM_DUMMY_VIRTUAL(160); ENUM_DUMMY_VIRTUAL(161);
    ENUM_DUMMY_VIRTUAL(162); ENUM_DUMMY_VIRTUAL(163); ENUM_DUMMY_VIRTUAL(164);
    ENUM_DUMMY_VIRTUAL(165); ENUM_DUMMY_VIRTUAL(166); ENUM_DUMMY_VIRTUAL(167);
    ENUM_DUMMY_VIRTUAL(168); ENUM_DUMMY_VIRTUAL(169); ENUM_DUMMY_VIRTUAL(170);
    ENUM_DUMMY_VIRTUAL(171); ENUM_DUMMY_VIRTUAL(172);
};

class CfEnumActor : public CfEnumActorBase {
public:
    virtual bool filter() = 0; // vtable +0x2BC
    u32 field_004;
    u8 tagState;
    u8 _pad_009[0x3374 - 0x009];
    u32 flags3374;
    u8 _pad_3378[0x3E9C - 0x3378];
    u8 moveObject;
    u8 _pad_3E9D[0x3F60 - 0x3E9D];
    CfEnumActorAux* actorAux;

    CfEnumObject* getMoveObject() {
        return reinterpret_cast<CfEnumObject*>(&moveObject);
    }
};

#undef ENUM_DUMMY_VIRTUAL

struct CfEnumKind {
    u8 _pad_00[0x91];
    u8 kind91;
};

extern "C" {
void* func_800B07E8__Fv();
CfEnumObject* func_800B7034();
CfEnumObject* func_800B7058(CfEnumObject*);
CfEnumObject* func_800B76A4();
CfEnumObject* func_800B77E4(CfEnumObject*);
CfEnumObject* func_800B77BC();
CfEnumObject* func_800B7934(CfEnumObject*);
CfEnumObject* func_800B76CC();
CfEnumObject* func_800B781C(CfEnumObject*);
CfEnumObject* func_800B76F4();
CfEnumObject* func_800B7854(CfEnumObject*);
CfEnumObject* func_800B771C();
CfEnumObject* func_800B788C(CfEnumObject*);
CfEnumObject* func_800B7744();
CfEnumObject* func_800B78C4(CfEnumObject*);
CfEnumObject* func_800B776C();
CfEnumObject* func_800B78FC(CfEnumObject*);
CfEnumObject* func_800B7794();
CfEnumObject* func_800B796C(CfEnumObject*);
CfEnumActor* func_8016FE34(CfEnumObject*);
CfEnumActor* func_800BBC0C(CfEnumObject*);
int func_80148778(void*, int);
int func_80051B38(CfEnumActorAux*);
int func_8009CF8C(int);
CfEnumKind* func_800AB3D0(CfEnumObject*);
void* getInstance__Q22cf14CBattleManagerFv();
int func_800DA06C(void*, CfEnumActor*);
CfEnumActor* func_800EA384(void*);
CfEnumActor* func_800EA3AC(void*, CfEnumObject*);
CfEnumObject* func_800FE68C();
CfEnumObject* func_800B708C__Fi(int);
CfEnumObject* getPlayer__Q22cf13CfGameManagerFi(int);
extern const float lbl_eu_80666EB8;
extern const float lbl_eu_80666EBC;
}

#define ENUM_ADD_OBJECT(info_, object_)                                      \
    do {                                                                     \
        (info_).objectId = (object_)->objectId;                              \
        (info_).object = (object_);                                          \
        (info_).field_18 = false;                                            \
        self->mObjInfo[self->mObjInfoCount++] = (info_);                     \
        self->mPtrArray[self->mPtrCount++] =                                 \
            &self->mObjInfo[self->mObjInfoCount - 1];                        \
    } while (false)

#define ENUM_BASIC_FILTERS(rejected_, object_, options_, compare_)           \
    do {                                                                     \
        if (!(rejected_) && !((options_) & 1) &&                             \
            !(object_)->isEnabled()) {                        \
            (rejected_) = true;                                              \
        }                                                                    \
        if (!(rejected_) && ((options_) & 2) &&                              \
            (!(object_)->filterA() ||                         \
             (compare_) != (object_)->filterC())) {           \
            (rejected_) = true;                                              \
        }                                                                    \
    } while (false)

#define ENUM_ACTOR_CORE_FILTERS(rejected_, object_, options_, zero_, one_) \
    do {                                                                   \
        if (!(rejected_) && !((options_) & 1) &&                           \
            !(object_)->isEnabled()) {                      \
            (rejected_) = true;                                            \
        }                                                                  \
        if (!(rejected_) && ((options_) & 2) &&                            \
            (!(object_)->filterA() ||                       \
             (zero_) != (object_)->filterC() ||             \
             (one_) != (object_)->filterD() ||              \
             !(object_)->filterB())) {                      \
            (rejected_) = true;                                            \
        }                                                                  \
    } while (false)

#define ENUM_ACTOR_FILTERS(rejected_, object_, actor_, options_, zero_, one_) \
    do {                                                                      \
        ENUM_ACTOR_CORE_FILTERS(rejected_, object_, options_, zero_, one_);   \
        if (!(rejected_) && ((options_) & 0x100) &&                           \
            func_80148778(&(actor_)->tagState, 0xE5)) {                       \
            (rejected_) = true;                                               \
        }                                                                     \
        if (!(rejected_) && ((options_) & 0x200)) {                           \
            if (func_80148778(&(actor_)->tagState, 0xE6)) {                   \
                (rejected_) = true;                                           \
            } else if ((actor_)->actorAux == 0 ||                  \
                       !func_80051B38((actor_)->actorAux)) {        \
                (rejected_) = true;                                           \
            }                                                                 \
        }                                                                     \
        if (!(rejected_) && ((options_) & 0x800)) {                           \
            if ((actor_)->filter()) {                           \
                (rejected_) = true;                                           \
            }                                                                 \
            if (func_80148778(&(actor_)->tagState, 0xF8)) {                   \
                (rejected_) = true;                                           \
            }                                                                 \
        }                                                                     \
        if (!(rejected_) && ((options_) & 0x1000) &&                          \
            (actor_)->filter() &&                               \
            (actor_)->actorAux->test(0)) {                          \
            (rejected_) = true;                                               \
        }                                                                     \
        if (!(rejected_) && ((options_) & 0x2000) &&                          \
            !func_800DA06C(getInstance__Q22cf14CBattleManagerFv(), actor_)) { \
            (rejected_) = true;                                               \
        }                                                                     \
    } while (false)

void func_800F4B5C(cf::CfObjEnumList* self, int flags, u32 options) {
    cf::CfObjEnumList::sObjInfo info0;
    cf::CfObjEnumList::sObjInfo info1;
    cf::CfObjEnumList::sObjInfo info2;
    cf::CfObjEnumList::sObjInfo info3;
    cf::CfObjEnumList::sObjInfo info4;
    cf::CfObjEnumList::sObjInfo info5;
    cf::CfObjEnumList::sObjInfo info6;
    cf::CfObjEnumList::sObjInfo info7;
    cf::CfObjEnumList::sObjInfo info8;
    cf::CfObjEnumList::sObjInfo info9;
    cf::CfObjEnumList::sObjInfo info10;
    cf::CfObjEnumList::sObjInfo info11;
    cf::CfObjEnumList::sObjInfo info12;
    cf::CfObjEnumList::sObjInfo info13;
    cf::CfObjEnumList::sObjInfo info14;
    cf::CfObjEnumList::sObjInfo info15;
    cf::CfObjEnumList::sObjInfo info16;
    cf::CfObjEnumList::sObjInfo info17;
    cf::CfObjEnumList::sObjInfo info18;

    if (flags == 0) {
        return;
    }

    if (flags & 1) {
        func_800B07E8__Fv();
        CfEnumObject* object = func_800B7034();
        float zero = lbl_eu_80666EB8;
        float one = lbl_eu_80666EBC;
        while (object != 0) {
            bool rejected = false;
            if (!(options & 1) && !object->isEnabled()) {
                rejected = true;
            }
            if (!rejected && (options & 2) &&
                (!object->filterA() ||
                 zero != object->filterC())) {
                rejected = true;
            }
            if (!rejected && flags == 0x200 &&
                (object->filterD() < one ||
                 (object->flags64 & 0x4800))) {
                rejected = true;
            }
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B7058(object);
                continue;
            }
            ENUM_ADD_OBJECT(info0, object);
            func_800B07E8__Fv();
            object = func_800B7058(object);
        }
        return;
    } else if (flags & 4) {
        CfEnumObject* selector = func_800FE68C();
        CfEnumObject* object = *(CfEnumObject**)((u8*)selector + 0x90E4);
        if (object != 0) {
            object = func_800B708C__Fi((int)object);
            ENUM_ADD_OBJECT(info1, object);
        }
        flags &= ~4u;
    } else if (flags & 0x10) {
        CfEnumObject* object = getPlayer__Q22cf13CfGameManagerFi(0);
        if (object != 0) {
            ENUM_ADD_OBJECT(info2, object);
        }
        flags &= ~0x10u;
    } else if (flags & 0x40000) {
        func_800F4B5C(self, 4, options);
        if (self->mPtrCount == 0) {
            func_800F4B5C(self, 0x10, options);
        }
        flags &= ~0x40000u;
    } else if (flags & 0x20) {
        func_800B07E8__Fv();
        CfEnumObject* object = func_800B76A4();
        float one = lbl_eu_80666EBC;
        float zero = lbl_eu_80666EB8;
        while (object != 0) {
            bool rejected = false;
            CfEnumActor* actor = func_8016FE34(object);
            if (!(object->flags64 & 2)) {
                rejected = true;
            }
            ENUM_ACTOR_CORE_FILTERS(rejected, object, options, zero, one);
            if (!rejected && (options & 0x100) &&
                func_80148778(&actor->tagState, 0xE5)) {
                rejected = true;
            }
            if (!rejected && (options & 0x200)) {
                if (func_80148778(&actor->tagState, 0xE6)) {
                    rejected = true;
                } else if (actor->actorAux == 0 ||
                           !func_80051B38(actor->actorAux)) {
                    rejected = true;
                }
            }
            if (!rejected && (options & 0x800)) {
                if (actor->filter()) {
                    rejected = true;
                }
                if (func_80148778(&actor->tagState, 0xF8)) {
                    rejected = true;
                }
            }
            if (!rejected && (options & 0x1000) &&
                actor->filter()) {
                rejected = true;
            }
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B77E4(object);
                continue;
            }
            ENUM_ADD_OBJECT(info3, object);
            func_800B07E8__Fv();
            object = func_800B77E4(object);
        }
        flags &= ~0x20u;
    } else if (flags & 0x100000) {
        CfEnumObject* object = getPlayer__Q22cf13CfGameManagerFi(1);
        if (object != 0) {
            ENUM_ADD_OBJECT(info4, object);
        }
        flags &= ~0x100000u;
    } else if (flags & 0x200000) {
        CfEnumObject* object = getPlayer__Q22cf13CfGameManagerFi(2);
        if (object != 0) {
            ENUM_ADD_OBJECT(info5, object);
        }
        flags &= ~0x200000u;
    } else if (flags & 8) {
        func_800B07E8__Fv();
        CfEnumObject* object = func_800B77BC();
        float zero = lbl_eu_80666EB8;
        while (object != 0) {
            bool rejected = false;
            if (!(object->flags64 & 0x200)) {
                rejected = true;
            }
            ENUM_BASIC_FILTERS(rejected, object, options, zero);
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B7934(object);
                continue;
            }
            ENUM_ADD_OBJECT(info6, object);
            break;
        }
        flags &= ~8u;
    } else if (flags & 0x100) {
        func_800B07E8__Fv();
        CfEnumObject* object = func_800B76CC();
        float one = lbl_eu_80666EBC;
        while (object != 0) {
            bool rejected = false;
            CfEnumActor* actor = func_8016FE34(object);
            if (!(object->flags64 & 4)) {
                rejected = true;
            }
            if (!rejected && (actor->flags3374 & 0x2000)) {
                rejected = true;
            }
            if (!rejected && !(options & 1) &&
                (!object->isEnabled() ||
                 !(actor->flags3374 & 0x08000000))) {
                rejected = true;
            }
            if (!rejected && (options & 2) &&
                (!object->filterA() ||
                 one != object->filterD() ||
                 !object->filterB())) {
                rejected = true;
            }
            if (!rejected && (options & 0x100) &&
                func_80148778(&actor->tagState, 0xE5)) {
                rejected = true;
            }
            if (!rejected && (options & 0x200)) {
                if (func_80148778(&actor->tagState, 0xE6)) {
                    rejected = true;
                } else if (actor->actorAux == 0 ||
                           !func_80051B38(actor->actorAux)) {
                    rejected = true;
                }
            }
            if (!rejected && (options & 0x800)) {
                if (actor->filter()) {
                    rejected = true;
                }
                if (func_80148778(&actor->tagState, 0xF8)) {
                    rejected = true;
                }
            }
            if (!rejected && (options & 0x1000) &&
                actor->filter() &&
                actor->actorAux->test(0)) {
                rejected = true;
            }
            if (!rejected && (options & 0x2000) &&
                !func_800DA06C(getInstance__Q22cf14CBattleManagerFv(), actor)) {
                rejected = true;
            }
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B781C(object);
                continue;
            }
            ENUM_ADD_OBJECT(info7, object);
            func_800B07E8__Fv();
            object = func_800B781C(object);
        }
        flags &= ~0x100u;
    } else if (flags & 0x400000) {
        func_800B07E8__Fv();
        CfEnumObject* object = func_800B76CC();
        float one = lbl_eu_80666EBC;
        while (object != 0) {
            bool rejected = false;
            CfEnumActor* actor = func_8016FE34(object);
            if (!(object->flags64 & 4)) {
                rejected = true;
            }
            if (!rejected && !(actor->flags3374 & 0x2000)) {
                rejected = true;
            }
            if (!rejected && !(options & 1) &&
                (!object->isEnabled() ||
                 !(actor->flags3374 & 0x08000000))) {
                rejected = true;
            }
            if (!rejected && (options & 2) &&
                (!object->filterA() ||
                 one != object->filterD() ||
                 !object->filterB())) {
                rejected = true;
            }
            if (!rejected && (options & 0x2000) &&
                !func_800DA06C(getInstance__Q22cf14CBattleManagerFv(), actor)) {
                rejected = true;
            }
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B781C(object);
                continue;
            }
            ENUM_ADD_OBJECT(info8, object);
            func_800B07E8__Fv();
            object = func_800B781C(object);
        }
        flags &= ~0x400000u;
    } else if (flags & 0x200) {
        func_800B07E8__Fv();
        CfEnumObject* object = func_800B76F4();
        float one = lbl_eu_80666EBC;
        float zero = lbl_eu_80666EB8;
        while (object != 0) {
            bool rejected = false;
            CfEnumActor* actor = func_800BBC0C(object);
            if (!(object->flags64 & 8)) {
                rejected = true;
            }
            ENUM_ACTOR_CORE_FILTERS(rejected, object, options, zero, one);
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B7854(object);
                continue;
            }
            ENUM_ADD_OBJECT(info9, object);
            func_800B07E8__Fv();
            object = func_800B7854(object);
        }
        flags &= ~0x200u;
    } else if (flags & 0x400) {
        func_800B07E8__Fv();
        CfEnumObject* object = func_800B771C();
        float zero = lbl_eu_80666EB8;
        while (object != 0) {
            bool rejected = false;
            if (!(object->flags64 & 0x80)) {
                rejected = true;
            }
            ENUM_BASIC_FILTERS(rejected, object, options, zero);
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B788C(object);
                continue;
            }
            ENUM_ADD_OBJECT(info10, object);
            func_800B07E8__Fv();
            object = func_800B788C(object);
        }

        func_800B07E8__Fv();
        object = func_800B76CC();
        float one = lbl_eu_80666EBC;
        while (object != 0) {
            bool rejected = false;
            CfEnumActor* actor = func_8016FE34(object);
            if (!(object->flags64 & 4)) {
                rejected = true;
            }
            if (!rejected && (actor->flags3374 & 0x2000)) {
                rejected = true;
            }
            if (!rejected && !(options & 1) && !object->isEnabled()) {
                rejected = true;
            }
            if (!rejected && (options & 2) &&
                (!object->filterA() ||
                 zero != object->filterC() ||
                 one != object->filterD() ||
                 !object->filterB())) {
                rejected = true;
            }
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B781C(object);
                continue;
            }
            ENUM_ADD_OBJECT(info11, object);
            func_800B07E8__Fv();
            object = func_800B781C(object);
        }
        flags &= ~0x400u;
    } else if (flags & 0x80000) {
        func_800B07E8__Fv();
        CfEnumObject* object = func_800B7744();
        float zero = lbl_eu_80666EB8;
        while (object != 0) {
            bool rejected = false;
            if (!(object->flags64 & 0x20)) {
                rejected = true;
            }
            ENUM_BASIC_FILTERS(rejected, object, options, zero);
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B78C4(object);
                continue;
            }
            ENUM_ADD_OBJECT(info12, object);
            func_800B07E8__Fv();
            object = func_800B78C4(object);
        }
        flags &= ~0x80000u;
    } else if (flags & 0x800) {
        func_800B07E8__Fv();
        CfEnumObject* object = func_800B776C();
        float zero = lbl_eu_80666EB8;
        while (object != 0) {
            bool rejected = false;
            if (!(object->flags64 & 0xC000)) {
                rejected = true;
            }
            ENUM_BASIC_FILTERS(rejected, object, options, zero);
            if (!rejected && (options & 0x8000) &&
                (u16)(object->type8C - 0x12F) <= 1) {
                rejected = true;
            }
            if (!rejected && !func_8009CF8C(0x3354)) {
                CfEnumKind* kind = func_800AB3D0(object);
                if (kind != 0 && kind->kind91 == 6) {
                    rejected = true;
                }
            }
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B78FC(object);
                continue;
            }
            ENUM_ADD_OBJECT(info13, object);
            func_800B07E8__Fv();
            object = func_800B78FC(object);
        }
        flags &= ~0x800u;
    } else if (flags & 0x1000) {
        func_800B07E8__Fv();
        CfEnumObject* object = func_800B7794();
        float zero = lbl_eu_80666EB8;
        while (object != 0) {
            bool rejected = false;
            if (!(object->flags64 & 0x100)) {
                rejected = true;
            }
            ENUM_BASIC_FILTERS(rejected, object, options, zero);
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B796C(object);
                continue;
            }
            ENUM_ADD_OBJECT(info14, object);
            func_800B07E8__Fv();
            object = func_800B796C(object);
        }
        flags &= ~0x1000u;
    } else if (flags & 0x4000) {
        void* manager = getInstance__Q22cf14CBattleManagerFv();
        CfEnumActor* source = func_800EA384(manager);
        CfEnumObject* object = source != 0 ? source->getMoveObject() : 0;
        float one = lbl_eu_80666EBC;
        float zero = lbl_eu_80666EB8;
        while (object != 0) {
            bool rejected = false;
            CfEnumActor* actor = func_8016FE34(object);
            ENUM_ACTOR_FILTERS(rejected, object, actor, options, zero, one);
            if (rejected) {
                manager = getInstance__Q22cf14CBattleManagerFv();
                source = func_800EA3AC(manager, object);
                object = source != 0 ? source->getMoveObject() : 0;
                continue;
            }
            ENUM_ADD_OBJECT(info15, object);
            manager = getInstance__Q22cf14CBattleManagerFv();
            source = func_800EA3AC(manager, object);
            object = source != 0 ? source->getMoveObject() : 0;
        }
        flags &= ~0x4000u;
    } else if (flags & 0x8000) {
        void* manager = getInstance__Q22cf14CBattleManagerFv();
        CfEnumActor* source = func_800EA384(manager);
        CfEnumObject* object = source != 0 ? source->getMoveObject() : 0;
        float one = lbl_eu_80666EBC;
        float zero = lbl_eu_80666EB8;
        while (object != 0) {
            bool rejected = false;
            CfEnumActor* actor = func_8016FE34(object);
            if (!(object->flags64 & 2)) {
                rejected = true;
            }
            ENUM_ACTOR_FILTERS(rejected, object, actor, options, zero, one);
            if (rejected) {
                manager = getInstance__Q22cf14CBattleManagerFv();
                source = func_800EA3AC(manager, object);
                object = source != 0 ? source->getMoveObject() : 0;
                continue;
            }
            ENUM_ADD_OBJECT(info16, object);
            manager = getInstance__Q22cf14CBattleManagerFv();
            source = func_800EA3AC(manager, object);
            object = source != 0 ? source->getMoveObject() : 0;
        }
        flags &= ~0x8000u;
    } else if (flags & 0x10000) {
        void* manager = getInstance__Q22cf14CBattleManagerFv();
        CfEnumActor* source = func_800EA384(manager);
        CfEnumObject* object = source != 0 ? source->getMoveObject() : 0;
        float one = lbl_eu_80666EBC;
        float zero = lbl_eu_80666EB8;
        while (object != 0) {
            bool rejected = false;
            CfEnumActor* actor = func_8016FE34(object);
            if (object->flags64 & 4) {
                rejected = true;
            }
            ENUM_ACTOR_FILTERS(rejected, object, actor, options, zero, one);
            if (rejected) {
                manager = getInstance__Q22cf14CBattleManagerFv();
                source = func_800EA3AC(manager, object);
                object = source != 0 ? source->getMoveObject() : 0;
                continue;
            }
            ENUM_ADD_OBJECT(info17, object);
            manager = getInstance__Q22cf14CBattleManagerFv();
            source = func_800EA3AC(manager, object);
            object = source != 0 ? source->getMoveObject() : 0;
        }
        flags &= ~0x10000u;
    } else if (flags & 0x20000) {
        void* manager = getInstance__Q22cf14CBattleManagerFv();
        CfEnumActor* source = func_800EA384(manager);
        CfEnumObject* object = source != 0 ? source->getMoveObject() : 0;
        while (object != 0) {
            bool rejected = false;
            func_800BBC0C(object);
            if (object->flags64 & 8) {
                rejected = true;
            }
            if (!rejected && !(options & 1) && !object->isEnabled()) {
                rejected = true;
            }
            if (rejected) {
                manager = getInstance__Q22cf14CBattleManagerFv();
                source = func_800EA3AC(manager, object);
                object = source != 0 ? source->getMoveObject() : 0;
                continue;
            }
            ENUM_ADD_OBJECT(info18, object);
            manager = getInstance__Q22cf14CBattleManagerFv();
            source = func_800EA3AC(manager, object);
            object = source != 0 ? source->getMoveObject() : 0;
        }
        flags &= ~0x20000u;
    } else {
        return;
    }

    func_800F4B5C(self, flags, options);
}

#undef ENUM_ACTOR_FILTERS
#undef ENUM_ACTOR_CORE_FILTERS
#undef ENUM_BASIC_FILTERS
#undef ENUM_ADD_OBJECT

void func_800F6D50(){}

void func_800F6E08(){}

void func_800F6ED0(){}

void func_800F7DEC(){}

void func_800F8794(){}

void func_800F8890(){}

void func_800F89DC(){}

void func_800F9AEC(){}

void __ct__800FA9B4(){}

void __ct__800FAE3C(){}

void __ct__800FB044(){}

void func_800FB270(){}

void func_800FB5AC(){}

void __ct__800FBA18(){}

void __ct__800FBF08(){}

void __ct__800FC040(){}

void __ct__800FC19C(){}

void __ct__800FC32C(){}

void __ct__800FC4FC(){}

void __ct__800FC648(){}

void __ct__800FC790(){}

void __ct__800FC8DC(){}

void __ct__800FCA24(){}

void __ct__800FCB80(){}

void __ct__800FCD10(){}

void __ct__800FCF1C(){}

void __ct__800FD0B4(){}

void __ct__800FD250(){}

void func_800FD2FC(){}

void func_800FD378(){}

void func_800FD3FC(){}

void func_800FD68C(void* self){}

void func_800FD698(void* self) { ((void(*)(void*))func_800FD68C)((char*)self - 0x604); }

extern "C" void func_800FD6A0(u8* self) { ((void(*)(void*))func_800F4798)((char*)self - 0x604); }

cf::CfObjectSelectorObj::~CfObjectSelectorObj() {}
