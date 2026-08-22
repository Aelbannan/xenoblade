#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/plugin/pluginVoice.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"
#include "monolib/util.hpp"

extern reslist<cf::CfObject*>* func_800B6BA4();

// Retail data labels referenced by voice_play.
extern const char lbl_eu_80513988[];
extern PluginFuncData lbl_eu_8053B880[];
extern float lbl_eu_80669008;
extern float lbl_eu_8066900C;

int voice_play(VMThread* pThread) {
    int voiceId = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int vol = vmArgIntGet(3, vmArgPtrGet(pThread, 2));

    reslist<cf::CfObject*>* list = func_800B6BA4();
    u8* object;

    for(reslist<cf::CfObject*>::iterator it = list->begin(); it != list->end(); it++){
        // List items point at the embedded CfObjectMove (base+0x3E9C);
        // recover the owning actor base (NULL stays NULL).
        u8* object = (u8*)*it;
        if(object != 0){
            object -= 0x3E9C;
        }
        if(((cf::CfObjectPc*)object)->CActorParam_UnkVirtualFunc138() == 0){
            if(voiceId == ((VoiceActorVoiceId*)object)->field_3F28){
                ((cf::CfObject*)(object + 0x3E9C))->func_800BE898(vol, 0x14, lbl_eu_80669008, lbl_eu_8066900C);
                break;
            }
        }
    }

    return 0;
}

/// Registers the "voice" plugin with the YGG VM, linking script function names
/// ("play") to C handler implementations.
void pluginVoiceRegist(){
    vmPluginRegist(lbl_eu_80513988, lbl_eu_8053B880);
}