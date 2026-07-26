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

void GetLightSet__Q34nw4r3g3d12LightSettingFi(void *result, void *self, int index) { unsigned short count = *(unsigned short *)((unsigned char *)self + 2); *(void **)result = self; if (index >= 0 && index < (int)count) *(void **)((unsigned char *)result + 4) = (unsigned char *)*(void **)((unsigned char *)self + 0xC) + index * 12; else *(void **)((unsigned char *)result + 4) = 0; }
extern "C" int Attach__Q34nw4r3g3d6AnmScnFiPQ34nw4r3g3d9AnmScnRes(void) { return 0x0; }
extern "C" int Detach__Q34nw4r3g3d6AnmScnFi(int) { return 0; }
extern "C" unsigned int GetResLightSetNumEntries__Q34nw4r3g3d9ResAnmScnCFv(const void*);

unsigned int GetNumLightSet__Q34nw4r3g3d9AnmScnResCFv(const void* this_ptr) {
    return GetResLightSetNumEntries__Q34nw4r3g3d9ResAnmScnCFv((const char*)this_ptr + 0x20);
}
extern "C" int GetResAnmAmbLightNumEntries__Q34nw4r3g3d9ResAnmScnCFv(const void*);

int GetNumAmbLight__Q34nw4r3g3d9AnmScnResCFv(const nw4r::g3d::AnmScnRes* ths) {
    return GetResAnmAmbLightNumEntries__Q34nw4r3g3d9ResAnmScnCFv((const char*)ths + 0x20);
}
extern "C" u32 GetResAnmLightNumEntries__Q34nw4r3g3d9ResAnmScnCFv(const void *);

u32 GetNumDiffuseLight__Q34nw4r3g3d9AnmScnResCFv(const void *self) {
    return GetResAnmLightNumEntries__Q34nw4r3g3d9ResAnmScnCFv((const void *)((const char *)self + 0x20));
}
unsigned short GetNumSpecularLight__Q34nw4r3g3d9AnmScnResCFv(const void* __this) {
    return *(const unsigned short*)(*(const unsigned char**)((const unsigned char*)__this + 0x20) + 0x36);
}
int GetNumFog__Q34nw4r3g3d9AnmScnResCFv(const void* this_) {
    extern int GetResAnmFogNumEntries__Q34nw4r3g3d9ResAnmScnCFv(const void*);
    return GetResAnmFogNumEntries__Q34nw4r3g3d9ResAnmScnCFv(reinterpret_cast<const char*>(this_) + 0x20);
}
extern "C" void GetResAnmCameraNumEntries__Q34nw4r3g3d9ResAnmScnCFv(void*);
extern "C" void GetNumCamera__Q34nw4r3g3d9AnmScnResCFv(void* self) { ((void(*)(void*))GetResAnmCameraNumEntries__Q34nw4r3g3d9ResAnmScnCFv)((char*)self + 0x20); }
extern "C" u16 GetLightSetMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv(const void* _this) {
    return *reinterpret_cast<const u16*>(*reinterpret_cast<const u8* const*>(reinterpret_cast<const u8*>(_this) + 0x20) + 0x3c);
}
unsigned short GetAmbLightMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv(const void* self) {
    const unsigned char* res = *(const unsigned char* const*)((const unsigned char*)self + 0x20);
    return *(const unsigned short*)(res + 0x3e);
}
int GetDiffuseLightMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv(const void* p) {
    return *(const unsigned short*)(*(const char* const*)((const char*)p + 0x20) + 0x40);
}
unsigned short GetFogMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv(const void* this_) {
    return *(const unsigned short*)(*(const char* const*)((const char*)this_ + 0x20) + 0x42);
}
u16 GetCameraMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv(const nw4r::g3d::AnmScnRes* pThis) {
    return *(const u16*)((const u8*)(*(const u32*)((const u8*)pThis + 0x20)) + 0x44);
}
extern "C" void Construct__Q34nw4r3g3d9AnmScnResFP12MEMAllocatorPUlQ34nw4r3g3d9ResAnmScnb() {}
extern "C" void SetFrame__Q34nw4r3g3d9AnmScnResFf() {}
float GetFrame__Q34nw4r3g3d9AnmScnResCFv(const void* this_ptr) {
    return *(const float*)((const unsigned char*)this_ptr + 0xC);
}
extern "C" void SetUpdateRate__Q34nw4r3g3d9AnmScnResFf() {}
float GetUpdateRate__Q34nw4r3g3d9AnmScnResCFv(const void* this_) { return *(const float*)((const char*)this_ + 0x10); }
extern "C" void UpdateFrame__Q34nw4r3g3d9AnmScnResFv() {}
extern "C" void G3dProc__Q34nw4r3g3d9AnmScnResFUlUlPv() {}
extern "C" void GetLightSet__Q34nw4r3g3d9AnmScnResFQ34nw4r3g3d8LightSetUl() {}
extern "C" void GetAmbLightColor__Q34nw4r3g3d9AnmScnResFUl() {}
extern "C" void GetLight__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d8LightObjPQ34nw4r3g3d8LightObjUl() {}
extern "C" void GetFog__Q34nw4r3g3d9AnmScnResFQ34nw4r3g3d3FogUl() {}
extern "C" void GetCamera__Q34nw4r3g3d9AnmScnResFQ34nw4r3g3d6CameraUl() {}
extern "C" void GetAmbLightResult__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d17AmbLightAnmResultUl() {}
extern "C" void GetLightResult__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d14LightAnmResultUl() {}
extern "C" void GetFogResult__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d12FogAnmResultUl() {}
extern "C" void GetCameraResult__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d15CameraAnmResultUl() {}
extern "C" void GetSpecularLightID__Q34nw4r3g3d9AnmScnResCFUl() {}
extern "C" void HasSpecularLight__Q34nw4r3g3d9AnmScnResCFUl() {}
extern "C" void UpdateCache__Q34nw4r3g3d9AnmScnResFv() {}
extern "C" void IsDerivedFrom__Q34nw4r3g3d9AnmScnResCFQ44nw4r3g3d6G3dObj7TypeObj() {}
extern "C" void IsDerivedFrom__Q34nw4r3g3d6AnmScnCFQ44nw4r3g3d6G3dObj7TypeObj() {}
extern "C" void GetTypeName__Q34nw4r3g3d9AnmScnResCFv() {}
extern "C" const char lbl_eu_8051D630[];

const void* GetTypeObj__Q34nw4r3g3d9AnmScnResCFv() {
    return lbl_eu_8051D630;
}
extern "C" void GetTypeName__Q34nw4r3g3d6AnmScnCFv() {}
extern "C" const char lbl_eu_8051D620[];
extern "C" void* GetTypeObj__Q34nw4r3g3d6AnmScnCFv(void) { return (void*)lbl_eu_8051D620; }
