// (CfObjectMove_setAnimModeArgs / testActorStatus now have single unified owner-header
// decls; no pre-include renames needed here.)
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/cf/object/CAIAction.hpp"
#undef testActorStatus
#include "kyoshin/plugin/pluginVoice.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"
#include "monolib/util.hpp"

// Retail data sections (all defined by this TU): .rodata plugin name "voice",
// .sdata2 voice-playback floats + entry name "play" + trailing pool floats
// (splitter's generic names), .data PluginFuncData table {name, func}
// terminated by {NULL, NULL}. voice_play reads the first two floats via
// sda21; the trailing 1.0f/0.5f ride in this TU's sdata2 pool (retail layout).
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_80513988[0x8] = "voice";
__declspec(section ".sdata2") __attribute__((used)) float lbl_eu_80669008 = 1.0f;
__declspec(section ".sdata2") __attribute__((used)) float lbl_eu_8066900C = 30.0f;
__declspec(section ".sdata2") __attribute__((used)) char lbl_eu_80669010[0x8] = "play";
__declspec(section ".sdata2") __attribute__((used)) float float_8066B818 = 1.0f;
__declspec(section ".sdata2") __attribute__((used)) float float_8066B81C = 0.5f;
__declspec(section ".data") __attribute__((used)) PluginFuncData lbl_eu_8053B880[2] = {
    { lbl_eu_80669010, voice_play },
    { 0, 0 },
};

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
                ((cf::CfObject*)(object + 0x3E9C))->requestVoice(vol, 0x14, lbl_eu_80669008, lbl_eu_8066900C);
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
