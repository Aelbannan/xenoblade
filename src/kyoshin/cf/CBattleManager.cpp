#include "kyoshin/cf/CBattleManager.hpp"

struct BMIf {
    virtual void _v008(); virtual void _v00C(); virtual void vf0010();
    virtual void _v014(); virtual void _v018(); virtual void _v01C(); virtual void _v020();
    virtual void vf0024();
};
#include "kyoshin/UnkClass_805764CC.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"
#include "kyoshin/cf/object/CfObjectEne.hpp"
#include "kyoshin/cf/CfSoundMan.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/work.hpp"

extern UNKTYPE* func_8009EC9C(u16 r3);
extern void func_8009D7E4(UNKTYPE* r3, u32 r4);
extern void* func_800B708C(int arg);

extern "C" void func_800EA484(cf::CBattleManager*, f32, int);

namespace cf{
    u32 CBattleManager::lbl_804F8228[] = {
        0,
        0x1AD,
        0x1AA,
        0x1B0,
        0x1B6,
        0x1BA,
        0x1B5,
        0x1B7,
        0x1B8,
        0x1B9
    };

    //Plays attack sound effects when a player character hits an enemy?
    void func_800D7A04(CfObjectPc* pObjectPc, CfObjectEne* pObjectEne){
        CActorParam_UnkStruct1* r3 = pObjectPc->CActorParam_UnkVirtualFunc129();
        u32 flags = r3->mFlagsArray[0].flags;

        if(flags & CActorParam_UnkStruct1::FLAG_BIT_1){
            CfSoundMan::func_801BFC38(0, 0x192, 0, 0, 0.6f);
            CfSoundMan::func_801BFC38(0, 0x191, 0, 0, 0.6f);
        }else if(flags & 0x2000){
            CfSoundMan::func_801BFC38(0, 0x1C5, 0, 0, 0.6f);
        }else{
            CActorParam_UnkStruct2* r30 = r3->unk50;

            if(r30 != nullptr){
                if(r30->unk78 & CActorParam_UnkStruct1::FLAG_BIT_24){
                    if(pObjectEne->CActorParam_UnkVirtualFunc19() == 1 ||
                    pObjectEne->CActorParam_UnkVirtualFunc19() == 2){
                        if(!(r30->unk78 & CActorParam_UnkStruct1::FLAG_BIT_9)){
                            CfSoundMan::func_801BFC38(0, 0x1B4, 0, 0, 0.6f);
                            return;
                        }
                    }else if(!(r30->unk78 & CActorParam_UnkStruct1::FLAG_BIT_9)){
                        CfSoundMan::func_801BFC38(0, 0x1B5, 0, 0, 0.6f);
                        return;
                    }
                }else if((r30->unk78 & CActorParam_UnkStruct1::FLAG_BIT_25)
                && !(r30->unk78 & CActorParam_UnkStruct1::FLAG_BIT_9)){
                    CfSoundMan::func_801BFC38(0, 0x1B5, 0, 0, 0.6f);
                    return;
                }
                
                if((s32)r30->unk40 == 1){
                    if(flags & CActorParam_UnkStruct1::FLAG_BIT_8){
                        CfSoundMan::func_801BFC38(0, 0x1AF, 0, 0, 0.6f);
                    }else{
                        //likely an inline
                        if(pObjectEne != nullptr && pObjectEne->unk64 & 0x2){
                            UNKTYPE* r3_1 = func_8009EC9C(pObjectEne->unk8C_3);
                            func_8009D7E4(&static_cast<UnkStruct_8009EC9C_Ret*>(r3_1)->unk1C, 1);
                        }

                        if(pObjectEne->CActorParam_UnkVirtualFunc19() == 1 ||
                        pObjectEne->CActorParam_UnkVirtualFunc19() == 2){
                            CfSoundMan::func_801BFC38(0, 0x1AE, 0, 0, 0.6f);
                        }else{
                            CfSoundMan::func_801BFC38(0, 0x1AD, 0, 0, 0.6f);
                        }
                    }
                }else if((s32)r30->unk40 == 2){
                    CfSoundMan::func_801BFC38(0, 0x1B7, 0, 0, 0.6f);
                }else{
                    CfSoundMan::func_801BFC38(0, 0x1B6, 0, 0, 0.6f);
                }
            }
        }
    }

    //Checks if an actor in the BattleEvent list matches a specific actor, used for damage/heal events?
    void func_800D7B80(u32 r31_actorIndex){
        //Loops through the BFEvent list and checks if mOwner's index is the given actor index
        struct BtlEventEntry {
            u32 mOwnerIndex;
            u8 pad_04[0x44];
            float field_0x48;
        };
        for(u32 i = 0; i < 0xD; i++){
            BtlEventEntry* entry = &((BtlEventEntry*)(CBattleManager::getInstance()->mBattleEventList.mList))[i];
            if(entry->mOwnerIndex == r31_actorIndex){
                entry->mOwnerIndex = 0;
                entry->field_0x48 = 0.0f;
            }
        }
    }

    void func_800D7C0C(CfObjectActor* actor, u32 bitMask){
        //stub
    }

    void func_800D7C38(CfObjectActor* actor, u32 bitMask){
        //stub
    }

    BOOL func_800D7C84(CfObjectActor* actor, u32 bitMask){
        return FALSE;
    }

    //Increment the count of a key in the battle manager's key-count mapping
    //(CBattleManager_Struct2 at +0x94)
    void func_800D7CB4(s32 key){
        CBattleManager_Struct2* map = &CBattleManager::getInstance()->unk94;
        u32 i = 0;
        
        while(i < 32){
            if(map->unk0[i].key == -1 || map->unk0[i].key == key){
                if(map->unk0[i].key == -1){
                    map->unk0[i].key = key;
                }

                map->unk0[i].count++;
                break;
            }

            i++;
        }
    }

    void func_800D7D00(u32 button){
        //stub
    }

    void func_800D7D18(){}

    // Sets the battle's time-limit remaining value and hands off to the timer
    // update routine stored at func_800EA484.
    void CBattleManager::func_800EA470() {
        // The 0.0f/1.0f compiled to sdata2 pool constants; their reloc names are
        // implicit pool labels (accepted at EQUIVALENT_MATCH).
        unk88 = 0.0f;
        func_800EA484(this, 1.0f, unk8C);
    }
}

namespace cf {
    CChainCombo::~CChainCombo() {
    }

    CChainEffect::~CChainEffect() {
    }

    CChainTime::~CChainTime() {
        func_8027CE30();
    }

    UnkClass_800D8DBC::~UnkClass_800D8DBC() {
    }
}

// Helper struct to access CBattleManager fields at known offsets
// (function reads +0x04, +0x78, +0xB8 from the argument)
struct CBM_Access {
    u8 pad_00[0x04];
    u32 field_0x04;
    u8 pad_08[0x70];
    u32 field_0x78;
    u8 pad_7C[0x3C];
    u32 field_0xB8;  // pointer to BtlTable
};

// Local struct for the table accessed through field_0xB8
struct BtlTable {
    u32 field_0x00;
    u32 compValue;     // +0x04: comparison key
    u32 groups[2][8];  // +0x08..+0x44: 2 groups, 8 u32 each (0x20 bytes per group)
};

//Checks battle event conditions and returns true(1)/false(0)
int func_800D7D24(void* self){
    int result = 0;
    
    if(!(((CBM_Access*)self)->field_0x78 & 1)){
        result = 1;
        return result;
    }
    
    BtlTable* table = (BtlTable*)(((CBM_Access*)self)->field_0xB8);
    if(table == NULL){
        return result;
    }
    
    u32 comp = table->compValue;
    u32* p = &table->groups[0][0];
    int matchFound = 0;
    int i;
    for(i = 0; i < 2; i++){
        if(p[0] == comp){ matchFound = 1; break; }
        if(p[1] == comp){ matchFound = 1; break; }
        if(p[2] == comp){ matchFound = 1; break; }
        if(p[3] == comp){ matchFound = 1; break; }
        if(p[4] == comp){ matchFound = 1; break; }
        if(p[5] == comp){ matchFound = 1; break; }
        if(p[6] == comp){ matchFound = 1; break; }
        if(p[7] == comp){ matchFound = 1; break; }
        p += 8;
    }
    
    if(matchFound && func_800B708C((int)comp) != NULL){
        return result;
    }
    
    u32 target = ((CBM_Access*)self)->field_0x04;
    for(i = 0; i < 16; i++){
        u32 slotVal = *(u32*)((u8*)self + 8 + i * 4);
        if(slotVal == 0) continue;
        if(slotVal == target){
            result = 1;
            return result;
        }
        return result;
    }
    return result;
}

void func_800D7EA0(){}
void func_800D81A8(){}
void func_800D9354(){}
void func_800D9978(){}
void func_800D9CA0(){}
void func_800DA0A4(){}
void func_800DB0FC(){}
void func_800DB4FC(){}
void func_800DB7F8(){}
void func_800DBA2C(){}
void func_800DBACC(){}