#include <nw4r/g3d.h>

#include <algorithm>

namespace nw4r {
namespace g3d {

void AnmScn::GetLightSetting(LightSetting* pSetting) {
    const u32 numLightSet = GetLightSetMaxRefNumber();
    const u32 numAmbLight = GetAmbLightMaxRefNumber();
    const u32 numDiffLight = GetDiffuseLightMaxRefNumber();

    if (numLightSet > 0) {
        const u32 numLightSetObj = pSetting->GetNumLightSet();
        const u32 numLoadableSet = std::min(numLightSet, numLightSetObj);

        for (u32 i = 0; i < numLoadableSet; i++) {
            LightSet set = pSetting->GetLightSet(i);
            GetLightSet(set, i);
        }
    }

    if (numAmbLight > 0) {
        AmbLightObj* pAmbObjArray = pSetting->GetAmbLightObjArray();
        const u32 numAmbObj = pSetting->GetNumLightObj();
        const u32 numLoadableAmb = std::min(numAmbLight, numAmbObj);

        for (u32 i = 0; i < numLoadableAmb; i++) {
            AmbLightObj* pAmbObj = &pAmbObjArray[i];
            *reinterpret_cast<u32*>(&pAmbObj->r) = GetAmbLightColor(i);
        }
    }

    if (numDiffLight > 0) {
        LightObj* pLightObjArray = pSetting->GetLightObjArray();
        const u32 numLightObj = pSetting->GetNumLightObj();
        const u32 numSpecLight = GetNumSpecularLight();

        const u32 numLight = numDiffLight + numSpecLight;
        const u32 numLoadableDiffLight = std::min(numDiffLight, numLightObj);
        const u32 numLoadableLight = std::min(numLight, numLightObj);

        for (u32 i = 0; i < numLoadableDiffLight; i++) {
            LightObj* pObj = &pLightObjArray[i];
            pObj->Disable();
        }

        for (u32 i = 0; i < numLoadableDiffLight; i++) {
            LightObj* pDiffObj = &pLightObjArray[i];
            LightObj* pSpecObj = NULL;

            if (pDiffObj->IsEnable()) {
                continue;
            }

            if (HasSpecularLight(i)) {
                const u32 specId = GetSpecularLightID(i);
                if (specId < numLoadableLight) {
                    pSpecObj = &pLightObjArray[specId];
                }
            }

            GetLight(pDiffObj, pSpecObj, i);
        }
    }
}

} // namespace g3d
} // namespace nw4r

// LLM-HARNESS-BEGIN: us-803e8620
extern "C" void GetLightSet_Q34nw4r3g3d12LightSettingFi() {}
// LLM-HARNESS-END: us-803e8620
// LLM-HARNESS-BEGIN: us-803e869c
extern "C" bool Attach__Q34nw4r3g3d6AnmScnFiPQ34nw4r3g3d9AnmScnRes() { return false; }
// LLM-HARNESS-END: us-803e869c
// LLM-HARNESS-BEGIN: us-803e86a4
int Detach__Q34nw4r3g3d6AnmScnFi(int) {
    return 0;
}
// LLM-HARNESS-END: us-803e86a4
// LLM-HARNESS-BEGIN: us-803e86e8
extern "C" unsigned int GetResLightSetNumEntries__Q34nw4r3g3d9ResAnmScnCFv(const void*);

unsigned int GetNumLightSet__Q34nw4r3g3d9AnmScnResCFv(const void* this_ptr) {
    return GetResLightSetNumEntries__Q34nw4r3g3d9ResAnmScnCFv((const char*)this_ptr + 0x20);
}
// LLM-HARNESS-END: us-803e86e8
// LLM-HARNESS-BEGIN: us-803e86f0
extern "C" int GetResAnmAmbLightNumEntries__Q34nw4r3g3d9ResAnmScnCFv(const void*);

int GetNumAmbLight__Q34nw4r3g3d9AnmScnResCFv(const nw4r::g3d::AnmScnRes* ths) {
    return GetResAnmAmbLightNumEntries__Q34nw4r3g3d9ResAnmScnCFv((const char*)ths + 0x20);
}
// LLM-HARNESS-END: us-803e86f0
// LLM-HARNESS-BEGIN: us-803e86f8
extern "C" u32 GetResAnmLightNumEntries__Q34nw4r3g3d9ResAnmScnCFv(const void *);

u32 GetNumDiffuseLight__Q34nw4r3g3d9AnmScnResCFv(const void *self) {
    return GetResAnmLightNumEntries__Q34nw4r3g3d9ResAnmScnCFv((const void *)((const char *)self + 0x20));
}
// LLM-HARNESS-END: us-803e86f8
// LLM-HARNESS-BEGIN: us-803e8700
unsigned short GetNumSpecularLight__Q34nw4r3g3d9AnmScnResCFv(const void* __this) {
    return *(const unsigned short*)(*(const unsigned char**)((const unsigned char*)__this + 0x20) + 0x36);
}
// LLM-HARNESS-END: us-803e8700
// LLM-HARNESS-BEGIN: us-803e870c
int GetNumFog__Q34nw4r3g3d9AnmScnResCFv(const void* this_) {
    extern int GetResAnmFogNumEntries__Q34nw4r3g3d9ResAnmScnCFv(const void*);
    return GetResAnmFogNumEntries__Q34nw4r3g3d9ResAnmScnCFv(reinterpret_cast<const char*>(this_) + 0x20);
}
// LLM-HARNESS-END: us-803e870c
// LLM-HARNESS-BEGIN: us-803e8714
extern "C" bool GetNumCamera__Q34nw4r3g3d9AnmScnResCFv() { return false; }
// LLM-HARNESS-END: us-803e8714
// LLM-HARNESS-BEGIN: us-803e871c
extern "C" u16 GetLightSetMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv(const void* _this) {
    return *reinterpret_cast<const u16*>(*reinterpret_cast<const u8* const*>(reinterpret_cast<const u8*>(_this) + 0x20) + 0x3c);
}
// LLM-HARNESS-END: us-803e871c
// LLM-HARNESS-BEGIN: us-803e8728
unsigned short GetAmbLightMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv(const void* self) {
    const unsigned char* res = *(const unsigned char* const*)((const unsigned char*)self + 0x20);
    return *(const unsigned short*)(res + 0x3e);
}
// LLM-HARNESS-END: us-803e8728
// LLM-HARNESS-BEGIN: us-803e8734
int GetDiffuseLightMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv(const void* p) {
    return *(const unsigned short*)(*(const char* const*)((const char*)p + 0x20) + 0x40);
}
// LLM-HARNESS-END: us-803e8734
// LLM-HARNESS-BEGIN: us-803e8740
unsigned short GetFogMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv(const void* this_) {
    return *(const unsigned short*)(*(const char* const*)((const char*)this_ + 0x20) + 0x42);
}
// LLM-HARNESS-END: us-803e8740
// LLM-HARNESS-BEGIN: us-803e874c
u16 GetCameraMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv(const nw4r::g3d::AnmScnRes* pThis) {
    return *(const u16*)((const u8*)(*(const u32*)((const u8*)pThis + 0x20)) + 0x44);
}
// LLM-HARNESS-END: us-803e874c
// LLM-HARNESS-BEGIN: us-803e8758
extern "C" void Construct__Q34nw4r3g3d9AnmScnResFP12MEMAllocatorPUlQ34nw4r3g3d9ResAnmScnb() {}
// LLM-HARNESS-END: us-803e8758
// LLM-HARNESS-BEGIN: us-803e8a38
extern "C" void SetFrame_Q34nw4r3g3d9AnmScnResFf() {}
// LLM-HARNESS-END: us-803e8a38
// LLM-HARNESS-BEGIN: us-803e8a90
float GetFrame__Q34nw4r3g3d9AnmScnResCFv(const void* this_ptr) {
    return *(const float*)((const unsigned char*)this_ptr + 0xC);
}
// LLM-HARNESS-END: us-803e8a90
// LLM-HARNESS-BEGIN: us-803e8a98
extern "C" void SetUpdateRate_Q34nw4r3g3d9AnmScnResFf() {}
// LLM-HARNESS-END: us-803e8a98
// LLM-HARNESS-BEGIN: us-803e8abc
float GetUpdateRate__Q34nw4r3g3d9AnmScnResCFv(const void* this_) { return *(const float*)((const char*)this_ + 0x10); }
// LLM-HARNESS-END: us-803e8abc
// LLM-HARNESS-BEGIN: us-803e8ac4
extern "C" void UpdateFrame__Q34nw4r3g3d9AnmScnResFv() {}
// LLM-HARNESS-END: us-803e8ac4
// LLM-HARNESS-BEGIN: us-803e8b3c
extern "C" void G3dProc__Q34nw4r3g3d9AnmScnResFUlUlPv() {}
// LLM-HARNESS-END: us-803e8b3c
// LLM-HARNESS-BEGIN: us-803e8b90
extern "C" void GetLightSet__Q34nw4r3g3d9AnmScnResFQ34nw4r3g3d8LightSetUl() {}
// LLM-HARNESS-END: us-803e8b90
// LLM-HARNESS-BEGIN: us-803e8cc8
extern "C" void* GetAmbLightColor_Q34nw4r3g3d9AnmScnResFUl() { return 0; }
// LLM-HARNESS-END: us-803e8cc8
// LLM-HARNESS-BEGIN: us-803e8d30
extern "C" void GetLight__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d8LightObjPQ34nw4r3g3d8LightObjUl() {}
// LLM-HARNESS-END: us-803e8d30
// LLM-HARNESS-BEGIN: us-803e8ec0
extern "C" void GetFog__Q34nw4r3g3d9AnmScnResFQ34nw4r3g3d3FogUl() {}
// LLM-HARNESS-END: us-803e8ec0
// LLM-HARNESS-BEGIN: us-803e8f70
extern "C" void GetCamera__Q34nw4r3g3d9AnmScnResFQ34nw4r3g3d6CameraUl() {}
// LLM-HARNESS-END: us-803e8f70
// LLM-HARNESS-BEGIN: us-803e90d0
extern "C" void GetAmbLightResult__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d17AmbLightAnmResultUl() {}
// LLM-HARNESS-END: us-803e90d0
// LLM-HARNESS-BEGIN: us-803e9164
extern "C" void GetLightResult__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d14LightAnmResultUl() {}
// LLM-HARNESS-END: us-803e9164
// LLM-HARNESS-BEGIN: us-803e91f0
extern "C" void GetFogResult__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d12FogAnmResultUl() {}
// LLM-HARNESS-END: us-803e91f0
// LLM-HARNESS-BEGIN: us-803e927c
extern "C" void GetCameraResult__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d15CameraAnmResultUl() {}
// LLM-HARNESS-END: us-803e927c
// LLM-HARNESS-BEGIN: us-803e9308
extern "C" void GetSpecularLightID_Q34nw4r3g3d9AnmScnResCFUl() {}
// LLM-HARNESS-END: us-803e9308
// LLM-HARNESS-BEGIN: us-803e9340
extern "C" void HasSpecularLight_Q34nw4r3g3d9AnmScnResCFUl() {}
// LLM-HARNESS-END: us-803e9340
// LLM-HARNESS-BEGIN: us-803e9384
extern "C" void UpdateCache__Q34nw4r3g3d9AnmScnResFv() {}
// LLM-HARNESS-END: us-803e9384
// LLM-HARNESS-BEGIN: us-803e94f8
extern "C" void IsDerivedFrom__Q34nw4r3g3d9AnmScnResCFQ44nw4r3g3d6G3dObj7TypeObj() {}
// LLM-HARNESS-END: us-803e94f8
// LLM-HARNESS-BEGIN: us-803e9548
extern "C" void IsDerivedFrom__Q34nw4r3g3d6AnmScnCFQ44nw4r3g3d6G3dObj7TypeObj() {}
// LLM-HARNESS-END: us-803e9548
// LLM-HARNESS-BEGIN: us-803e9580
extern "C" void GetTypeName__Q34nw4r3g3d9AnmScnResCFv() {}
// LLM-HARNESS-END: us-803e9580
// LLM-HARNESS-BEGIN: us-803e95b0
extern "C" const char lbl_eu_8051D630[];

const void* GetTypeObj__Q34nw4r3g3d9AnmScnResCFv() {
    return lbl_eu_8051D630;
}
// LLM-HARNESS-END: us-803e95b0
// LLM-HARNESS-BEGIN: us-803e95bc
extern "C" void GetTypeName__Q34nw4r3g3d6AnmScnCFv() {}
// LLM-HARNESS-END: us-803e95bc
// LLM-HARNESS-BEGIN: us-803e95ec
extern "C" void GetTypeObj__Q34nw4r3g3d6AnmScnCFv() {}
// LLM-HARNESS-END: us-803e95ec
