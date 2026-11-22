// (CfObjectMove_setAnimModeArgs / testActorStatus now have single unified owner-header
// decls; no pre-include renames needed here.)
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/cf/object/CAIAction.hpp"
#undef testActorStatus
#include "kyoshin/plugin/pluginVoice.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"
#include "monolib/util.hpp"

int voice_play(VMThread* pThread) {
    VoiceActorListNode* node;
    int voiceId = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int vol = vmArgIntGet(3, vmArgPtrGet(pThread, 2));

    VoiceActorList* list = getListB28();

    for(node = list->sentinel->next; node != list->sentinel; node = node->next){
        // Items point at the embedded CfObjectMove (base+0x3E9C);
        // recover the owning actor base (NULL stays NULL).
        u8* object = node->object != 0 ? node->object - 0x3E9C : node->object;
        if(((cf::CfObjectPc*)object)->CActorParam_isBattleLocked() == 0){
            if(voiceId == ((VoiceActorVoiceId*)object)->field_3F28){
                ((cf::CfObject*)(object + 0x3E9C))->requestVoice(vol, 0x14, 1.0f, 30.0f);
                break;
            }
        }
    }

    return 0;
}

// name-guess: method/function pairs passed to vmPluginRegist (retail .data
// object at 0x8053B880).
static PluginFuncData sVoiceFuncData[2] = {
    { "play", voice_play },
    { 0, 0 },
};

/// Registers the "voice" plugin with the YGG VM, linking script function names
/// ("play") to C handler implementations.
void pluginVoiceRegist(){
    vmPluginRegist("voice", sVoiceFuncData);
}
