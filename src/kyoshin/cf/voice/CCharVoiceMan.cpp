#include "kyoshin/cf/voice/CCharVoiceMan.hpp"
#include "kyoshin/UnkClass_805764CC.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "monolib/util/MemManager.hpp"

// CCharVoiceMan ctor symbol (placement construction of a heaped 0x234 buffer);
// kept external so the call site emits a real call rather than being inlined.
extern "C" cf::CCharVoiceMan* __ct__CCharVoiceMan(cf::CCharVoiceMan* self);

namespace cf{
    CCharVoiceMan::CCharVoiceMan(){
        unk204 = 0;
        unk208 = 0;
        unk20C = 0;
        unk210 = 0;
        unk214 = 1;
        unk218 = 0;
        unk21C = 0;
        unk220 = 0;
        unk221 = 0;
        unk222 = 0;
        unk223 = 0;
        unk224 = 0;
        unk229 = 0;
        unk22A = 0;
        unk22C = 0;
        unk230 = 0;
        UnkClass_805764CC* classPtr = UnkClass_805764CC::func_800B07E8();
        classPtr->func_800B8804(this);
    }

    CCharVoiceMan::~CCharVoiceMan(){
        UnkClass_805764CC* classPtr = UnkClass_805764CC::func_800B07E8();
        classPtr->func_800B88E0(this);
    }
}

// ── retail @sda21 globals ────────────────────────────────────────────────────
extern "C" u32 lbl_eu_80663E24;    // presentation/event bitfield (bit 0x00400000)
extern "C" void* lbl_eu_80663E14;  // scene / allocation-source handle
extern "C" u32 lbl_eu_80664A5C;    // global character-voice counter
extern "C" float lbl_eu_80668C68;  // single-precision reset value for unk218
cf::CCharVoiceMan* lbl_eu_80664A58; // CCharVoiceMan singleton

// Scene-model helper returning an allocation-region handle (mtl MemManager).
extern "C" mtl::ALLOC_HANDLE func_80496004(void* src);

// Sibling voice-module free functions (cvsys TUs) — un-mangled retail names.
extern "C" int func_802B03A4(void* self);
extern "C" cf::CSoundNode* func_802A9604();
extern "C" cf::CSoundNode* __ct__802A4E48();
extern "C" cf::CSoundNode* __ct__802A96C0();
// Additional retail node constructors / helpers used by the enqueue functions.
extern "C" cf::CSoundNode* func_802A8B6C();
extern "C" cf::CSoundNode* func_802B5970();
extern "C" cf::CSoundNode* __ct__CVS_THREAD_ORDER(int order);
extern "C" cf::CSoundNode* __ct__802A8DE8();
extern "C" cf::CSoundNode* func_802A6958();
extern "C" cf::CSoundNode* __ct__802A5830();
extern "C" cf::CSoundNode* __ct__802A8C04();
extern "C" cf::CSoundNode* func_802B8D4C();

bool func_802A1EA0() { return true; }

void func_802A1F9C() {
    cf::CCharVoiceMan* voice_man = lbl_eu_80664A58;
    if (voice_man != 0)
        voice_man->unk222 = 0;
}
// If the character-voice-disable flag is clear, flag the manager's byte 0x229.
void func_802A2CF0() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    lbl_eu_80664A58->unk229 = 1;
}
void func_802A34E4(){}
unsigned int func_802A35A0(unsigned int value) { unsigned int counter = lbl_eu_80664A5C; lbl_eu_80664A5C = counter + 1; return (counter << 16) | (value & 0xFFFF); }
int func_802A3740(void* self) { return 0; }

// Allocate the 0x234-byte CCharVoiceMan and store it as the singleton.
void func_802A14B8() {
    mtl::ALLOC_HANDLE handle = func_80496004(lbl_eu_80663E14);
    cf::CCharVoiceMan* man =
        (cf::CCharVoiceMan*)mtl::MemManager::allocate(0x234, handle);
    if (man != 0)
        man = __ct__CCharVoiceMan(man);
    lbl_eu_80664A58 = man;
    lbl_eu_80664A5C = 0;
}
void func_802A1500(){}
void func_802A1610(){}
void func_802A1C68(){}
void func_802A1D04(){}
void func_802A1DA8() {
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (m != 0) {
        m->FactoryEvent2();
        lbl_eu_80664A58 = 0;
    }
}
void func_802A1DF0(){}
void func_802A1EA8(){}
// Register a freshly-created sound node, then clear byte 0x22C.
void func_802A1FB4() {
    cf::CSoundNode* node = func_802A9604();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
    lbl_eu_80664A58->unk22C = 0;
}
// Append a freshly-created sound node to the manager's voice-event list.
void func_802A201C() {
    cf::CSoundNode* node = __ct__802A96C0();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A2078(){}
void func_802A216C(){}
// If the passed actor is active, teed a u32 from +0x3F10 into unk230.
void func_802A2210(cf::CVoiceActorInfo* self) {
    if (func_802B03A4(self) != 0) {
        lbl_eu_80664A58->unk230 = self->field_3F10;
    }
}
void func_802A2250(){}
void func_802A232C(){}
void func_802A2424(){}
void func_802A24B4(){}
void func_802A2558(){}
// If the passed actor's move-base is the current player, clear voice count 0x224.
void func_802A25EC(cf::CVoiceActorBase* actor) {
    if (!(lbl_eu_80663E24 & 0x00400000)) {
        const u8* movePc = (const u8*)actor;
        if (actor != 0)
            movePc = (const u8*)&actor->moveBase;
        if (movePc == (const u8*)cf::CfGameManager::getPlayer(0)) {
            lbl_eu_80664A58->unk224 = 0;
        }
    }
}
void func_802A2648(){}
void func_802A26D8(){}
void func_802A27F4() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__802A4E48();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A285C() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__802A4E48();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A28C4(int a, int b, int c) {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    if (b != c && c < b) {
        cf::CSoundNode* node = __ct__802A8DE8();
        cf::CCharVoiceMan* m = lbl_eu_80664A58;
        if (node != 0) {
            cf::CSoundNode* tail = m->unk210;
            if (tail != 0)
                tail->next = node;
            if (m->unk20C == 0) {
                m->unk20C = node;
                m->unk208 = (u32)node - (u32)&m->unk4[0];
            }
            m->unk210 = node;
        }
    }
}
// ── retail node constructors (cvsys TUs) — un-mangled retail names ──────────
extern "C" cf::CSoundNode* __ct__CVS_THREAD_PARTY_GAGE();
extern "C" cf::CSoundNode* __ct__802A6AA8();
extern "C" cf::CSoundNode* __ct__802A5B88();
extern "C" cf::CSoundNode* __ct__802A86CC();
extern "C" cf::CSoundNode* __ct__802A6E84();
extern "C" cf::CSoundNode* func_802B9064();
extern "C" cf::CSoundNode* func_802A4798();
extern "C" cf::CSoundNode* func_802AE38C();
extern "C" cf::CSoundNode* func_802A7674();
extern "C" cf::CSoundNode* func_802A8AC8();


// Append a freshly constructed sound node to the voice-event list.
void func_802A293C() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__CVS_THREAD_PARTY_GAGE();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A29A4() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__802A6AA8();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A2A0C() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__802A5B88();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A2A74() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__802A86CC();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A2ADC() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__802A6E84();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A2B44() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__CVS_THREAD_ORDER(0);
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A2BB0() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__CVS_THREAD_ORDER(1);
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A2C1C() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__CVS_THREAD_ORDER(2);
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A2C88() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802B9064();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A2D0C() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    if (lbl_eu_80664A58->unk229 == 0)
        return;
    cf::CSoundNode* node = func_802A6958();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A2D84() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CCharVoiceMan* m0 = lbl_eu_80664A58;
    m0->unk218 = lbl_eu_80668C68;
    lbl_eu_80664A58->unk224 = 0;
    lbl_eu_80664A58->unk228 = 0;
    lbl_eu_80664A58->unk22A = 0;
    cf::CSoundNode* node = func_802B8D4C();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
// If the battle-manager actor ring has any entries and the flag is clear,
// flag the manager's byte 0x22A.
void func_802A2E08() {
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    cf::CVoiceBattleNode* sent = ((cf::CBattleListAccessor*)bm)->list0;
    int count = 0;
    for (cf::CVoiceBattleNode* cur = sent->next; cur != sent; cur = cur->next)
        count++;
    if (count > 0 && !(lbl_eu_80663E24 & 0x00400000)) {
        lbl_eu_80664A58->unk22A = 1;
    }
}
void func_802A2E68(){}
void func_802A2EEC() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802A4798();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A2F54(){}
void func_802A300C() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802AE38C();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A3074() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802A7674();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A30DC() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802A8AC8();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A3144() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802A8B6C();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A31AC() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802B5970();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
int func_802A3214() {
    cf::CSoundNode* node = __ct__802A5830();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
        return (int)node->field_18;
    }
    return -1;
}
int func_802A3290() {
    cf::CSoundNode* node = __ct__802A8C04();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
        return (int)node->field_18;
    }
    return -1;
}
void func_802A330C(){}
void func_802A35B8(){}
void func_802A3680(){}
void func_802A3748(){}
void CCharVoiceMan_FactoryEvent2(){}
void func_802A38C8(){}
