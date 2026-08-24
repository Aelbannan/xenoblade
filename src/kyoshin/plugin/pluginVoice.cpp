// CAIAction.hpp (via CfObjectPc.hpp -> CfObjectActor.hpp) declares void*
// forms of func_800BE12C / func_80174C98 that conflict with the u8*/other
// forms in CfObjectMove.hpp etc. (MWCC 10197). This TU calls neither, so
// pre-include the u8* forms, then rename the conflicting decls out of the
// way for the include (CVision.cpp idiom).
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#define func_800BE12C pluginVoiceAiActionMoveDeclUnused
#define func_80174C98 pluginVoiceAiActionGateDeclUnused
#include "kyoshin/cf/object/CAIAction.hpp"
#undef func_800BE12C
#undef func_80174C98
#include "kyoshin/plugin/pluginVoice.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"
#include "monolib/util.hpp"

// Retail data labels referenced by voice_play.
extern const char lbl_eu_80513988[];
extern PluginFuncData lbl_eu_8053B880[];
extern float lbl_eu_80669008;
extern float lbl_eu_8066900C;

int voice_play(VMThread* pThread) {
    VoiceActorListNode* node;
    int voiceId = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int vol = vmArgIntGet(3, vmArgPtrGet(pThread, 2));

    VoiceActorList* list = func_800B6BA4();

    for(node = list->sentinel->next; node != list->sentinel; node = node->next){
        // Items point at the embedded CfObjectMove (base+0x3E9C);
        // recover the owning actor base (NULL stays NULL).
        u8* object = node->object != 0 ? node->object - 0x3E9C : node->object;
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
