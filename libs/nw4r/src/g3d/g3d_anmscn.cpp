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

void GetLightSet__Q34nw4r3g3d12LightSettingFi(void* result, void* self, int index) {
    nw4r::g3d::LightSetting* setting = static_cast<nw4r::g3d::LightSetting*>(self);
    nw4r::g3d::LightSet* lightSet = static_cast<nw4r::g3d::LightSet*>(result);
    *lightSet = setting->GetLightSet(index);
}
int Attach__Q34nw4r3g3d6AnmScnFiPQ34nw4r3g3d9AnmScnRes(void) { return 0x0; }
int Detach__Q34nw4r3g3d6AnmScnFi(int) { return 0; }
unsigned int GetResLightSetNumEntries__Q34nw4r3g3d9ResAnmScnCFv(const void*);

unsigned int GetNumLightSet__Q34nw4r3g3d9AnmScnResCFv(const void* this_ptr) {
    return GetResLightSetNumEntries__Q34nw4r3g3d9ResAnmScnCFv(
        reinterpret_cast<const char*>(this_ptr) + 0x20);
}
int GetResAnmAmbLightNumEntries__Q34nw4r3g3d9ResAnmScnCFv(const void*);

int GetNumAmbLight__Q34nw4r3g3d9AnmScnResCFv(const nw4r::g3d::AnmScnRes* ths) {
    return GetResAnmAmbLightNumEntries__Q34nw4r3g3d9ResAnmScnCFv(
        reinterpret_cast<const char*>(ths) + 0x20);
}
u32 GetResAnmLightNumEntries__Q34nw4r3g3d9ResAnmScnCFv(const void *);

u32 GetNumDiffuseLight__Q34nw4r3g3d9AnmScnResCFv(const void *self) {
    return GetResAnmLightNumEntries__Q34nw4r3g3d9ResAnmScnCFv(
        reinterpret_cast<const void*>(reinterpret_cast<const char*>(self) + 0x20));
}
unsigned short GetNumSpecularLight__Q34nw4r3g3d9AnmScnResCFv(const void* __this) {
    return *reinterpret_cast<const unsigned short*>(
        *reinterpret_cast<const unsigned char* const*>(
            reinterpret_cast<const unsigned char*>(__this) + 0x20) + 0x36);
}
int GetNumFog__Q34nw4r3g3d9AnmScnResCFv(const void* this_) {
    extern int GetResAnmFogNumEntries__Q34nw4r3g3d9ResAnmScnCFv(const void*);
    return GetResAnmFogNumEntries__Q34nw4r3g3d9ResAnmScnCFv(reinterpret_cast<const char*>(this_) + 0x20);
}
void GetResAnmCameraNumEntries__Q34nw4r3g3d9ResAnmScnCFv(void*);
void GetNumCamera__Q34nw4r3g3d9AnmScnResCFv(void* self) {
    reinterpret_cast<void(*)(void*)>(GetResAnmCameraNumEntries__Q34nw4r3g3d9ResAnmScnCFv)(
        static_cast<char*>(self) + 0x20);
}
u16 GetLightSetMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv(const void* _this) {
    return *reinterpret_cast<const u16*>(*reinterpret_cast<const u8* const*>(reinterpret_cast<const u8*>(_this) + 0x20) + 0x3c);
}
unsigned short GetAmbLightMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv(const void* self) {
    const unsigned char* res = *reinterpret_cast<const unsigned char* const*>(
        reinterpret_cast<const unsigned char*>(self) + 0x20);
    return *reinterpret_cast<const unsigned short*>(res + 0x3e);
}
int GetDiffuseLightMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv(const void* p) {
    return *reinterpret_cast<const unsigned short*>(
        *reinterpret_cast<const char* const*>(
            reinterpret_cast<const char*>(p) + 0x20) + 0x40);
}
unsigned short GetFogMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv(const void* this_) {
    return *reinterpret_cast<const unsigned short*>(
        *reinterpret_cast<const char* const*>(
            reinterpret_cast<const char*>(this_) + 0x20) + 0x42);
}
u16 GetCameraMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv(const nw4r::g3d::AnmScnRes* pThis) {
    return *reinterpret_cast<const u16*>(
        reinterpret_cast<const u8*>(
            *reinterpret_cast<const u32*>(reinterpret_cast<const u8*>(pThis) + 0x20)) + 0x44);
}
void Construct__Q34nw4r3g3d9AnmScnResFP12MEMAllocatorPUlQ34nw4r3g3d9ResAnmScnb(){}
void SetFrame__Q34nw4r3g3d9AnmScnResFf(){}
float GetFrame__Q34nw4r3g3d9AnmScnResCFv(const void* this_ptr) {
    return *(const float*)((const unsigned char*)this_ptr + 0xC);
}
void SetUpdateRate__Q34nw4r3g3d9AnmScnResFf(){}
float GetUpdateRate__Q34nw4r3g3d9AnmScnResCFv(const void* this_) { return *(const float*)((const char*)this_ + 0x10); }
void UpdateFrame__Q34nw4r3g3d9AnmScnResFv(){}
void G3dProc__Q34nw4r3g3d9AnmScnResFUlUlPv(){}
void GetLightSet__Q34nw4r3g3d9AnmScnResFQ34nw4r3g3d8LightSetUl(){}
void GetAmbLightColor__Q34nw4r3g3d9AnmScnResFUl(){}
void GetLight__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d8LightObjPQ34nw4r3g3d8LightObjUl(){}
void GetFog__Q34nw4r3g3d9AnmScnResFQ34nw4r3g3d3FogUl(){}
void GetCamera__Q34nw4r3g3d9AnmScnResFQ34nw4r3g3d6CameraUl(){}
void GetAmbLightResult__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d17AmbLightAnmResultUl(){}
void GetLightResult__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d14LightAnmResultUl(){}
void GetFogResult__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d12FogAnmResultUl(){}
void GetCameraResult__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d15CameraAnmResultUl(){}
void GetSpecularLightID__Q34nw4r3g3d9AnmScnResCFUl(){}
void HasSpecularLight__Q34nw4r3g3d9AnmScnResCFUl(){}
void UpdateCache__Q34nw4r3g3d9AnmScnResFv(){}
void IsDerivedFrom__Q34nw4r3g3d9AnmScnResCFQ44nw4r3g3d6G3dObj7TypeObj(){}
void IsDerivedFrom__Q34nw4r3g3d6AnmScnCFQ44nw4r3g3d6G3dObj7TypeObj(){}
void GetTypeName__Q34nw4r3g3d9AnmScnResCFv(){}
extern const char lbl_eu_8051D630[];

const void* GetTypeObj__Q34nw4r3g3d9AnmScnResCFv() {
    return lbl_eu_8051D630;
}
void GetTypeName__Q34nw4r3g3d6AnmScnCFv(){}
extern "C" const char lbl_eu_8051D620[];
extern "C" void* GetTypeObj__Q34nw4r3g3d6AnmScnCFv(void) { return (void*)lbl_eu_8051D620; }
