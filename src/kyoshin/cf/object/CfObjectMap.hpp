#pragma once

#include "kyoshin/cf/CfBdat.hpp"

#include <types.h>
#include "kyoshin/cf/object/CfObjectModel.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)
#include "monolib/util/FixStr.hpp"  // ml::FixStr<64> for Tok_FormatStr (matches IResInfo.hpp)
class CResLookup;
class CScnEnvLgtCtrlListItem;

class CScn;
class CView;

// External imports used by CfObjectMap.cpp: retail Fv/unmangled symbols
// are declared with their verbatim mangled names (PLAN.md §17.6).
// ---------------------------------------------------------------------------

// Map resource managers returned by CfRes_getInstPtr80/60/A8/FF0.
struct UnkRes866A0 {
    u32 field_0x0;         // 0x00 flags
    u32 field_0x4;         // 0x04
    u32 field_0x8;         // 0x08
    u8 field_0xC[0x14];    // 0x0C-0x1F
    u32 field_0x20;        // 0x20
    u32 field_0x24;        // 0x24
    u32 field_0x28;        // 0x28
    CResLookup* field_0x2C; // 0x2C
};

// Big embedded sub-object at +0xF0 (UnkClass_8047BB54 in retail).  Only the
// flag word at 0x10 is touched directly; the rest pads up to +0x2F2C.
struct UnkMapSubF0 {
    u8 field_0x0[0x10];
    u32 flags_0x10;                  // 0x100 (bit 0x8 toggled by func_800BA66C)
    u32 field_0x14;                  // 0x104
    u8 unk18[0x20 - 0x18];           // 0x108-0x10F
    u32 field_0x20;                  // 0x110
    u8 unk24[0x2E38 - 0x24];         // 0x114-0x2F27
    u32 field_0x2E38;                // 0x2F28
};

// Node-pool manager embedded at +0x2F2C (UnkClass_8047CD0C in retail).
struct UnkMapPool2F2C {
    u8 field_0x0[0xC];
};


#include "kyoshin/code_801F3BE0.hpp"
#include "kyoshin/cf/CfSoundMan.hpp"

namespace cf {
    class CfObjectMap;
}

namespace cf {
class CfMapEffectManager;
}

struct CScnEnvLgtCtrlResList;  // env-controller list owner (CScn::mLgtCtrl)

extern "C" {
    // ---- CTaskLOD task helpers ----
    void acquireLODResource__8CTaskLODFv(void* task, u32 value);
    void resetActiveLOD__8CTaskLODFv(void* task);
    void bindTaskToLOD__8CTaskLODFv(void* task);
    void setLODFilterFlag__8CTaskLODFv(void* task);
    void updateLODRange__8CTaskLODFv(u16 value, float amount);
    void syncLODTask__8CTaskLODFv(u32 value);

    // ---- embedded UnkClass_8047BB54 subobject (at +0xF0) ----
    void* initMpfSystem__17UnkClass_8047BB54Fv(void* self, void* r4, void* r5, void* r6);
    void setMpfFloatParam__17UnkClass_8047BB54Fv(void* self, float value);
    void forwardMpfCallA__17UnkClass_8047BB54Fv(void* self, u32 value);
    void resetMpfInstance__17UnkClass_8047BB54Fv(void* self);

    // ---- embedded UnkClass_8047CD0C node-pool manager (at +0x2F2C) ----
    void initFromConfig__17UnkClass_8047CD0CFv(void* self, void* r4);
    void initNodePool__17UnkClass_8047CD0CFv(void* self, void* r4, u32 r5, u32 r6);
    void clearPoolData__17UnkClass_8047CD0CFv(void* self);
    void resetPoolState__17UnkClass_8047CD0CFv(void* self);

    // ---- CfGameManager / map-effect helpers ----
    cf::CfMapEffectManager* getMapEffectManager__Q22cf13CfGameManagerFv();
    u32 getReloadParam2();
    u32 getReloadParam1();

    // ---- CTaskCulling statics ----
    void* func_801A2C04__Q22cf12CTaskCullingFv();
    void clearOccFrustums__Q22cf12CTaskCullingFv();
    void reloadOccFrustums__Q22cf12CTaskCullingFv();

    // ---- scene / resource helpers ----
    bool Scn_IsAnimActiveOrNull(void* ptr);
    int getFileSize__11CDeviceFileFPCc(const char* path, int flags);
    void* scnLgtCreateEnvController(CScnEnvLgtCtrlResList* mgr, const void* name, int flag);
    void scnLgtRemoveLgtItem(CScnEnvLgtCtrlResList* mgr, void* resource);
    void scnLgtPushLgtValue(CScnEnvLgtCtrlResList* mgr, u16 value, float amount);
    void* Scn_InitGlobalB(void* r3, void* r4, void* r5);
    void* scnImN4BuildByIdx(void* r3, void* r4, u32 r5, u32 r6, u32 r7, u32 r8);
    void CfModel_InstallSub(void* r3, void* r4);
    void* getHandleMEM1__Q23mtl10MemManagerFv();

    // ---- CfRes helpers ----
extern "C" u32 CfRes_getNameRecCount();
extern "C" void* CfRes_extType(void* r3, s16 r4, void* r5, void* r6, void* r7);
extern "C" void* CfRes_tryResolveLink(u32 a, u32 b, u32 c);
extern "C" void CfRes_tryResolveSlotF4(u32 a, u32 b, u32 c);
extern "C" u32 CfRes_tryResolveSlot130(u32 a, u32 b, u32 c, u32 d);
extern "C" UnkRes866A0* CfRes_getInstPtr80();
    UnkRes866A0* CfRes_getInstPtrBC();
extern "C" UnkRes866A0* CfRes_getInstPtrF8();
extern "C" UnkRes866A0* CfRes_getInstPtr134();
    void CfRes_cancelPendingRead(void* child, void* parent);
extern "C" void* CfRes_getLazyField20(void* r3, void* r4);
extern "C" void* CfRes_getLazyField10(void* r3, void* r4);
    void* KyoshinHeap_GetField18();
    void* KyoshinHeap_GetField1C();
    void* KyoshinHeap_AllocFld10(void* r3);
    void* KyoshinHeap_AllocFld14(void* r3);
    void* KyoshinHeap_AllocFld18(u32 size);
    void* KyoshinHeap_AllocFld34(void* r3);
    void* KyoshinHeap_GetField40();
extern "C" void CfRes_tryUpdateSlot16C(void* r3);

    int Tok_FormatStr(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);
    void CfRes_delegateOp2(void* r3, void* r4, void* r5);
    // getBdatStringColumnValue: canonical decl in plugin/ocBdat.hpp.

    // ---- this TU's own retail symbols (Fv-annotated / unmangled) ----
    void setMapEffectFlag__Q22cf11CfObjectMapFv(cf::CfObjectMap* self, void* task);
    int CfObjectMap_resolveResource(cf::CfObjectMap* self, u32 a, u32 b);
    void CfObjectMap_updateFrame(cf::CfObjectMap* self);
    void CfObjectMap_updateEvents(cf::CfObjectMap* self);
}

// Real C++ declaration from monolib/scn/CScnTimeApi.hpp.
void Scn_SetTimeScale(CScn* scene, float value);

// ---- data symbols
extern CView* gmCurrentView;     // global scene object (secondary)
extern u32 gmPauseFlags;        // global flag word
extern u32 lbl_eu_80664184;        // bdat row index

namespace cf {
    //size: 0x2F50
    class CfObjectMap : public CfObjectModel {
    public:
        //0x0-BE: CfObjectModel
        u8 field_0xBE[0xDC - 0xBE];        // 0xBE-0xDB
        void* field_0xDC;                  // 0xDC
        void* field_0xE0;                  // 0xE0
        void* field_0xE4;                  // 0xE4
        void* field_0xE8;                  // 0xE8
        void* field_0xEC;                  // 0xEC
        UnkMapSubF0 field_0xF0;            // 0xF0-0x2F2B (UnkClass_8047BB54)
        UnkMapPool2F2C field_0x2F2C;       // 0x2F2C-0x2F37 (UnkClass_8047CD0C)
        u32 field_0x2F38;                  // 0x2F38 packed map id
        CScnEnvLgtCtrlListItem* field_0x2F3C; // 0x2F3C map effect object
        u8 field_0x2F40;                   // 0x2F40
        u8 field_0x2F41;                   // 0x2F41
        u8 field_0x2F42[2];                // 0x2F42-0x2F43
        u32 field_0x2F44;                  // 0x2F44
        float field_0x2F48;                // 0x2F48
        u8 field_0x2F4C[0x2F50 - 0x2F4C];  // 0x2F4C-0x2F4F

        void* checkTarget();
        void cleanupMap();
        void setMapVisibility(u32 arg);
        void setMapId(unsigned long v);
        void dispatchMapState();
        virtual ~CfObjectMap();
        bool isObjectMapReady();
        void clearStatus();
        void setMapScale();
        void nopMap();
    };
} // namespace cf
