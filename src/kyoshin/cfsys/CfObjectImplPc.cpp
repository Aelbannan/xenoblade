// Auto-scaffolded catalog TU for kyoshin/cfsys/CfObjectImplPc
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/cfsys/CfObjectImplPc.hpp"

#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfSoundMan.hpp"

void func_800C891C(u8* self);  // same-TU target (defined below)

// Minimal cf::CBattleManager view (the full header conflicts with this TU's
// C-ABI import declarations). Only getInstance / +0x194 / +0x1A8 are needed.
namespace cf {
class CBattleManager {
public:
    virtual ~CBattleManager();
    static CBattleManager* getInstance();
    u8 _pad04[0x194 - 0x04];
    u8 unk194[0x1A8 - 0x194];        // party-gauge sub-object
    u8 mChain[0x283D8 - 0x1A8];      // chain gauge block at +0x1A8
};
} // namespace cf

// Player init/refresh: prepares via func_800CA948, dispatches vtable slot
// 0xE4 and feeds its result + sub-fields into func_8015BB3C, binds the +0x28
// holder to the battle object's +0x3E9C sub-object, and when the +0x70 token
// formats with entry id 7, allocates + constructs a CPcEffect07 whose +4
// pointer becomes field_378. Finally resets the battle object's idle flag
// word (vtable 0x27C result) and notifies the +0x3E9C sub-object (vtable
// 0x20C) when the battle id at +0x3F28 is 1.
void func_800C5998(cf::CfObjectImplPc* self)
{
    func_800CA948((u8*)self);
    func_8015BB3C(self->vfE4(), (u8*)self->field_14, (u8*)self->field_18);
    u8* owner = (u8*)self->field_18;
    if (owner != 0) {
        owner += 0x3e9c;
    }
    func_802A0B8C((u8*)&self->field_28, owner);
    u32 out14;
    u32 out10;
    u32 outC;
    u32 out8;
    func_800AA318(self->field_14->field_70, &out14, &out10, &outC, &out8);
    if (out10 == 7) {
        u8* mem = (u8*)mtl::MemManager::allocate(0x64, func_80061FE8());
        if (mem != 0) {
            u8* actor = (u8*)self->field_14;
            if (actor != 0) {
                actor -= 0x3e9c;
            }
            mem = __ct__cf_CPcEffect07(mem, actor);
        }
        if (mem != 0) {
            mem = (u8*)mem + 4;
        }
        self->field_378 = (cf::CfObjectImplPcSub*)mem;
    }
    if (self->field_18->field_3F28 == 1) {
        cf::CfObjectImplPc27C* p = self->field_18->vf27C();
        p->field_00 = 0;
        self->field_18->mSub.sf20C(0x27);
    }
}

void func_800C5AA0(cf::CfObjectImplPc* self)
{
    if (self->field_378 != 0) {
        if (self->field_378 != 0) {
            self->field_378->vf08(1);
        }
        self->field_378 = 0;
    }
    func_800CAA44(self);
}

void func_800C5B00(){}

void func_800C6A58(){}

void func_800C6EC0(cf::CfObjectImplPc* self, u32 param)
{
    if (self->field_378 != 0 && self->field_18->field_3F2C != 0) {
        self->field_378->vf10();
    }
    func_800CEE28(self, param);
}

// Arts-command feedback: when the battle-id gate passes and the 0x400000
// battle flag is clear, dispatch the command id (0xEA..0xF2 -> action ids)
// to the battle object's +0x3E9C sub-object (vtable 0x20C), then forward the
// original args to func_800CB9AC.
void func_800C6F30(cf::CfObjectImplPc* self, int arg2, int arg3, int arg4)
{
    if (arg2 != 0 && (u32)arg2 != (u32)self->field_18->field_3F60) {
        return;
    }
    cf::CfObjectImplPc18* battleObj = func_800BFC68((cf::CfObjectMove*)(
        self->field_18 != 0 ? (cf::CfObjectImplPc18*)((u8*)self->field_18 + 0x3E9C)
                            : (cf::CfObjectImplPc18*)0));
    char* name = 0;
    int flag = 0;
    switch (arg3) {
    case 0:
    {
        // Laid out to match retail's dispatch order: tests, default jump,
        // slot-0 body, jump, slot-1 body, then the shared name check.
        if (arg4 == 0) {
            flag = 0;
            name = func_800BEDC4((u8*)&battleObj->mSub, 0);
        } else if (arg4 == 1) {
            flag = 1;
            name = func_800BEDC4((u8*)&battleObj->mSub, 1);
        }
        if (strcmp(name, lbl_eu_804FC758) == 0) {
            name = 0;
        }
        if (name != 0) {
            battleObj->mSub.sf104(name, flag);
            battleObj->field_45B0 = 1;
        }
        break;
    }
    case 1:
    {
        if (arg4 == 0) {
            flag = 0;
            name = func_800BED80((u8*)&battleObj->mSub, 0);
        } else if (arg4 == 1) {
            flag = 1;
            name = func_800BED80((u8*)&battleObj->mSub, 1);
        }
        if (strcmp(name, lbl_eu_804FC758) == 0) {
            name = 0;
        }
        if (name != 0) {
            battleObj->mSub.sf104(name, flag);
            battleObj->field_45B0 = 0;
        }
        break;
    }
    }
}

void func_800C70BC(){}

void func_800C75D4(){}

void func_800C819C(){}

// Battle-actor scan: clears the +0x37C count, recounts actors whose +0x3E9C
// sub-object's action target (vtable 0x4C) matches the battle object's
// +0x3F10 id, and when the count grew, cancels/refreshes the actor's actions
// (vtable 0x210 / 0x208 / 0x204) and plays a sound if nothing was active.
// Returns 1 when the count grew, else 0.
int func_800C86E8(cf::CfObjectImplPc* self)
{
    self->field_18->field_04->vf20(0x400000);
    self->field_18->field_04->vf20(0x800000);
    u8 holder[8];
    func_80043D90(holder);
    func_800F4A98(func_80043F18(holder), 0x80000002, 0);
    u32 startId = self->vf40();
    func_800F6ED0(func_80043F18(holder), startId);
    for (u32 i = 0; i < ((cf::CfEnumList*)func_80043F18(holder))->field_620; i++) {
        cf::CfObjectImplPc18* obj = (cf::CfObjectImplPc18*)func_8016FE34(
            func_800F6EAC(func_80043F18(holder), i));
        self->field_18->vf2C4((u8*)obj, lbl_eu_80666BCC, lbl_eu_80666BCC, lbl_eu_80666BCC);
    }
    u8* listObj = func_800F6E08(func_80043F18(holder));
    __dt__80043E88(holder, -1);
    if (self->field_18->mSub.sf4C() == 0) {
        self->field_18->mSub.sf50((u32)listObj);
        if (self->field_18->mSub.sf4C() == 0) {
            cf::CfObjectImplPc18* pobj = (cf::CfObjectImplPc18*)func_8016FE34(
                (u8*)func_800B708C((int)func_800BFC68(cf::CfGameManager::getPlayer(0))->mSub.sf4C()));
            if (pobj != 0) {
                u32 id = *pobj->field_04->vf30();
                if (func_80174C98((u8*)pobj, &id, 0x100000) != 0) {
                    self->field_18->mSub.sf50(pobj->field_3F10);
                    self->field_18->vf2C4((u8*)pobj, lbl_eu_80666BCC, lbl_eu_80666BCC, lbl_eu_80666BCC);
                }
            }
        }
    }
    func_800C891C((u8*)self);
    self->vf78();
    return 0;
}

void func_800C891C(u8* self){}

void func_800C969C(){}

// Event dispatch (func_800C9A20): gate on the event id, run the arts-command
// path for ids 0xEA/0xF0/0x10A, then chain-gauge handling and the +0x290
// notification pipeline.
void func_800C9A20(cf::CfObjectImplPc* self, CfObjectImplPcEvt* evt)
{
    if (func_80145F78(evt->field_0C)) {
        if (evt->field_2E != 0 || (evt->field_30 & 0x40000000) != 0) {
            func_800F3970(cf::CBattleManager::getInstance(), self->field_18, 0, 0x15, 0);
        }
    }
    if (evt->field_0C == 0x10a) {
        // slot 0x204: (id, 0, 0x61, 0, 0)
        self->field_18->mSub.sf204(0x1a, 0, 0x61, 0, 0);
    }
    if (evt->field_0C == 0xea) {
        func_800BF29C(&self->field_18->mSub, 0x66, lbl_eu_80666BC8, 0, lbl_eu_80666BF8, 0);
    } else if (evt->field_0C == 0xf0) {
        func_800BF29C(&self->field_18->mSub, 0x66, lbl_eu_80666BC8, 0, lbl_eu_80666BF8, 0);
    } else if (evt->field_0C == 0x10a) {
        func_800BF29C(&self->field_18->mSub, 0x66, lbl_eu_80666BC8, 0, lbl_eu_80666BF8, 0);
    }
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    if (func_802799F0(&bm->mChain, self->field_18) == 0) {
        u16 id = evt->field_0C;
        cf::CfObjectImplPc18* obj = self->field_18;
        if ((u32)(id - 0xF) <= 1 || id == 9 || id == 0xB) {
            u32 state = obj->vf308();
            if (state == 4) {
                obj->vf304(3);
                cf::CfObjectImplPc2F4* t = obj->vf2F4();
                // MWCC int->double conversion of a byte selected by t's s16 offset,
                // scaled and halved toward zero.
                int half = (s32)(t->field_10 * ((u8*)t)[t->field_02 + 4]) / 2;
                obj->vf2FC(half);
            } else if (state == 1) {
                obj->vf304(0);
            } else {
                func_800F3970(cf::CBattleManager::getInstance(), obj, 0, 0x11, 0);
            }
        }
        s32 gauge;
        if (obj->vf290() != 0 && func_80260264(obj->vf290(), 0x63, &gauge) != 0 &&
            func_80145C00(evt->field_0C)) {
            func_8018C820(&cf::CBattleManager::getInstance()->unk194, gauge);
        }
    }
    func_800CAB30(self, evt);
}

void func_800C9CEC(){}

// Arts command dispatch: promotes id 0xCF to 0xD0 while the battle object's
// +0x3374 bit-0x8000 flag is set, then asks func_80148778 whether the arts
// container holds the id; when missing, bounces to the object's vtable[0x94].
void func_800CA084(cf::CfObjectImplPc* self, u32 param)
{
    if (param == 0xcf && (self->field_18->field_3374 & 0x8000) != 0) {
        param = 0xd0;
    }
    if (func_80148778(&self->field_18->field_08, (int)param) == 0) {
        self->vf94(param);
    }
}

// Arts command dispatch (jump table): when the 0x400000 battle flag is clear,
// map the command id 0xEA..0xF2 onto action ids sent to the battle object's
// +0x3E9C sub-object (vtable 0x20C), then forward the args to func_800CB9AC.
void func_800CA104(cf::CfObjectImplPc* self, u32 param)
{
    cf::CfGameManager::getInstance();
    // Retail loads lis r3, 0x400 -> the flag word passed is 0x04000000.
    if (func_8006EF04(0x04000000) != 0) {
        return;
    }
    switch (param - 0xea) {
    case 0:
        self->field_18->mSub.sf20C(0x15);
        break;
    case 1:
        self->field_18->mSub.sf20C(0x16);
        break;
    case 2:
        self->field_18->mSub.sf20C(0x18);
        break;
    case 3:
        self->field_18->mSub.sf20C(0x17);
        break;
    case 4:
        self->field_18->mSub.sf20C(0x19);
        break;
    case 5:
        self->field_18->mSub.sf20C(0x1a);
        break;
    case 6:
        self->field_18->mSub.sf20C(0x1b);
        break;
    case 7:
        self->field_18->mSub.sf20C(0x16);
        break;
    // Grouping 0x22 with case 8 raises the jump-table bounds to retail's
    // 35-entry table without emitting an extra block (an empty case would be
    // folded into default).
    case 8:
    case 0x22:
        self->field_18->mSub.sf20C(0x27);
        break;
    }
    func_800CB9AC((u8*)self, param);
}

void func_800CEA34(void*);

void func_800CA274(void* self, int value)
{
    CfObjectImplPcSubObj* sub = static_cast<CfObjectImplPcSubObj*>(static_cast<CfObjectImplPcData*>(self)->mSubObj);
    if (sub->mSomeValue != 0 && value != 0) {
        sub->mSomeValue = value;
    }
    func_800CEA34(self);
}

// Battle-actor recount: clears the +0x37C count, recounts actors whose
// +0x3E9C sub-object's action target (vtable 0x4C) matches the battle
// object's +0x3F10 id, and when the count grew, cancels/refreshes the
// actor's actions (vtable 0x210 / 0x208 / 0x204) and plays a sound if
// nothing was active. Returns 1 when the count grew, else 0.
int func_800CA294(cf::CfObjectImplPc* self)
{
    u32 prev = self->field_37C;
    self->field_37C = 0;
    u8 holder[8];
    func_80043D90(holder);
    func_800F4A98(func_80043F18(holder), 0x80000000, 0x800);
    for (u32 i = 0; i < ((cf::CfEnumList*)func_80043F18(holder))->field_620; i++) {
        cf::CfObjectImplPc18* obj = (cf::CfObjectImplPc18*)func_8016FE34(
            func_800F6EAC(func_80043F18(holder), i));
        if (obj->mSub.sf4C() == self->field_18->field_3F10) {
            self->field_37C++;
        }
    }
    if (self->field_37C > prev) {
        u32 v = self->field_18->mSub.sf210(0x11);
        self->field_18->mSub.sf208(0x11);
        self->field_18->mSub.sf204(0x11, 0, -1, 0, 0);
        if (v == 0) {
            cf::CfSoundMan::func_801BFC38(0, 0x5c, 0, 0, lbl_eu_80666BC8);
        }
        __dt__80043E88(holder, -1);
        return 1;
    }
    if (self->field_37C == 0) {
        self->field_18->mSub.sf208(0x11);
    }
    __dt__80043E88(holder, -1);
    return 0;
}

// Tail-calls the +0x378 sub-object's vtable[0x0C] when the sub-object
// exists, forwarding args 1, 3, 4, 5 (arg 2 is not forwarded).
void func_800CA42C(cf::CfObjectImplPc* self, u32 a, u32 b, u32 c, u32 d, u32 e)
{
    if (self->field_378 != 0) {
        self->field_378->vf0C(a, c, d, e);
    }
}

// Per-frame player update: runs func_800CEBE0, raises the battle object's
// +0x3F60 flag bit 0x400000, and while the +0x14 object is in state 8 past
// scenario 0x167, notifies the +0x98 sub-object with the string at +0x3B.
void func_800CA458(cf::CfObjectImplPc* self)
{
    func_800CEBE0(self);
    self->field_18->field_3F60->field_0C |= 0x400000;
    if (self->field_14->field_8C == 8) {
        if (func_800822F4__Q22cf13CfGameManagerFv() >= 0x167) {
            self->field_14->field_98->vf28((const char*)(lbl_eu_804FC758 + 0x3b), 0);
        }
    }
}

// Adjuster thunk for CfObjectImplPc virtual destructor at vtable slot this-0xc.
// Adjusts this pointer to the complete object, then tail-calls the real destructor.
extern "C" void func_800CA4DC(void* self)
{
    __dt__Q22cf14CfObjectImplPcFv((u8*)self - 0xc);
}

// Adjuster thunk for CfObjectImplPc virtual destructor at vtable slot this-0x10.
// Adjusts this pointer to the complete object, then tail-calls the real destructor.
extern "C" void func_800CA4E4(void* self)
{
    __dt__Q22cf14CfObjectImplPcFv((u8*)self - 0x10);
}
