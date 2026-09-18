// CREvtModel - Real-time event model base class
// High-level C/C++ reconstruction

#include "kyoshin/realtimeevt/CREvtModel.hpp"
#include "libs/monolib/src/scn/CScnItemModel.hpp"
#include <nw4r/g3d/res/g3d_resfile.h>
#include <nw4r/g3d/res/g3d_resanmchr.h>
#include <nw4r/g3d/res/g3d_resdict.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// External declarations (C++ linkage for non-mangled symbols)
extern "C" {
extern void __ct__cf_CREvtObj(void* self, int arg);
extern void* __dt__Q22cf8CREvtObjFv(void* self, int dealloc);
extern void __dt__80185754(void* self);
extern void EvtObj_RunCallback(void* self);
extern void* EvtObj_AllocBlock(u32 size);
extern u32 Scn_IsAnimActiveOrNull(void* ptr);
extern void* Scn_InitGlobalA(void* global, void* data, int param);
extern void func_8016AF4C(void* data, const char* name, int* outValue);
extern void EvtSeqFindResFloat(void* data, const char* name, void* outValue);
extern int  EvtSeqCheckRegionStatus(void* data);
extern u32  REvtCam_GetFieldA5Flag(void);
extern u32  REvtCam_IsField98Set(void);
extern int EvtSeqGetCounter104(void);
extern void ScnEnvLgt_EnableFlag100(void* ptr, int flag);
extern void* simGetLeafActData(void* model);
extern void simSetFlag2OnTree(void* model, int flag);
extern void func_80482DF4(void* model, int flag);
extern void func_804839D4(void* model, u32 r4, void* r5, u32 r6, u32 r7, u32 r8, u32 r9);
extern void simSetValue7E8(void* model, int value);
extern void func_80482B3C(void* model, int flag);
extern void* GetResAnmChr__Q34nw4r3g3d7ResFileCFi(void* resFile, int index);
extern void* GetResUserData__Q34nw4r3g3d9ResAnmChrFv(void* self);
extern void* __vc__Q34nw4r3g3d6ResDicCFPCc(void* self, const char* key);
extern void Panic__Q24nw4r2dbFPCciPCce(const char* file, int line, const char* fmt, ...);
extern void* __dynamic_cast(void* obj, long offset, const void* srcType, const void* dstType, void* src2dst);
extern void __ct__CREvtModelMap(void* self, void* parent);
extern void __ct__CREvtModelObj(void* self, void* parent);
extern void __ct__CREvtModelPc(void* self, void* parent);
extern void ItemAnim_GetChr_E708(void* data, int index);
extern const void* lbl_eu_8053167C[];
extern const void* lbl_eu_806623F8[2];
extern const void* lbl_eu_80662400[2];
// typified split1 retail .rodata: RTTI type names + nw4r option strings as
// one 8-aligned struct (two commented halves). Byte-exact; offsets used by
// code: 0x00 CScnMaruShadowNw4r, 0x14 IScnPolyShadow, 0x24 cf::CREvtModel,
// 0x34 cf::CREvtObj, blob 0x44 (MonoChrome+0, MonoRGB+0x0B, noDynamics+0x13,
// LgtID+0x1E, HidePT+0x3D, core+0x44).
struct Rodata_CREvtModel {
    char head[0x24];  // 80503300 + gap + 80503314 + gap
    char tail[0x6C];  // 80503324 + gap + 80503334 + gaps + 80503344 blob
};
__declspec(section ".rodata") __attribute__((aligned(8))) const struct Rodata_CREvtModel rodata_CREvtModel __attribute__((used)) = {
    "CScnMaruShadowNw4r\000\000IScnPolyShadow\000",
    "cf::CREvtModel\000\000cf::CREvtObj\000\000\000\000Monochrome\000MonoRGB\000noDynamics\000LgtID\000SdwType\000SdwRadius\000SdwPow\000HidePT\000core"
};
// rodata names below are UNDEF reads (like the .data-backed halves): MWCC
// 8-packs .rodata objects, so retail's contiguous 144-byte pool can only be
// rebuilt as one neutral struct. 80503344 is a local macro (offset 0x44).
#define lbl_eu_80503344 (rodata_CREvtModel.tail + 0x20)
extern const float lbl_eu_80667770;
extern const float lbl_eu_80667774;
extern const float lbl_eu_80667778;
extern const float lbl_eu_8066777C;
extern const void* lbl_eu_806623F8[2];
extern const void* lbl_eu_80662400[2];
extern const void* lbl_eu_80662418;
extern const void* lbl_eu_8066241C;
extern const char lbl_eu_80530D18[];
extern const char lbl_eu_80530CFC[];
extern const char lbl_eu_80530CF0[];
extern const char lbl_eu_80530F08[];
extern const char lbl_eu_80530EE0[];
extern const char lbl_eu_80530D54[];
extern const char lbl_eu_80530D2C[];
extern const char lbl_eu_80530DC4[];
extern const char lbl_eu_80530DA8[];
extern const char lbl_eu_80530D68[];
extern const char lbl_eu_80530ECC[];
extern const char lbl_eu_80530E88[];
}

#define FLD(type, base, off) (*(type*)((char*)(base) + (off)))

// All functions with C linkage
extern "C" {

void* __ct__CREvtModel(void* self, void* pData, void* pArg) {
    __ct__cf_CREvtObj(self, 3);
    u32* base = (u32*)self;
    u32* vtable = (u32*)(void*)&lbl_eu_8053167C;
    u32 zero = 0;
    base[0] = (u32)vtable;
    base[0x14/4] = (u32)pArg;
    base[0x1C/4] = (u32)pData;
    base[0x20/4] = zero;
    base[0x24/4] = zero;
    base[0x28/4] = zero;
    base[0x2C/4] = 0;
    base[0x30/4] = 0;
    base[0x34/4] = 0;
    base[0x18/4] = base[0x20/4] | 0x8;
    return self;
}

void* __ct__80172668(void* self, int deallocFlag) {
    if (self != 0) {
        FLD(const void**, self, 0) = lbl_eu_8053167C;
        reinterpret_cast<CREvtModel*>(self)->reset();
        __dt__Q22cf8CREvtObjFv(self, 0);
        if (deallocFlag > 0) { __dt__80185754(self); }
    }
    return self;
}

void func_80172768(void* self) {
    reinterpret_cast<CREvtModel*>(self)->vfunc_08();
    void* p24 = FLD(void*, self, 0x24);
    if (p24 != 0) { Scn_IsAnimActiveOrNull(p24); FLD(void*, self, 0x24) = 0; }
    void* p20 = FLD(void*, self, 0x20);
    if (p20 != 0) { Scn_IsAnimActiveOrNull(p20); FLD(void*, self, 0x20) = 0; }
}

int func_801726DC(void* self) {
    u32 flags = FLD(u32, self, 0x18);
    if (flags & 0x80) { return 1; }
    reinterpret_cast<CREvtModel*>(self)->vfunc_08();
    void* p24 = FLD(void*, self, 0x24);
    if (p24 != 0) { Scn_IsAnimActiveOrNull(p24); FLD(void*, self, 0x24) = 0; }
    void* p20 = FLD(void*, self, 0x20);
    if (p20 != 0) { Scn_IsAnimActiveOrNull(p20); FLD(void*, self, 0x20) = 0; }
    FLD(u32, self, 0x18) |= 0x80;
    return 1;
}

void* evtModelDataPtr(void* self) {
    return (char*)FLD(void*, self, 0x1C) + 0x10;
}

void func_801727DC(void* self) {
    EvtObj_RunCallback(self);
    s32 counter = FLD(s32, self, 0x34);
    if (counter < 4) {
        u16 val = *(u16*)((char*)FLD(void*, self, 0x1C) + counter * 2 + 0x6C);
        if (val != 0) {
            int result = EvtSeqGetCounter104();
            if (result == (int)val) {
                u32 flags = FLD(u32, self, 0x18);
                if (flags & 0x400) { flags &= ~0x400; }
                else { flags |= 0x400; }
                FLD(u32, self, 0x18) = flags;
                FLD(s32, self, 0x34) = counter + 1;
            }
        }
    }
    void* model = FLD(void*, self, 0x20);
    if (model != 0) {
        u32 flags = FLD(u32, self, 0x18);
        int bit = (flags >> 21) & 1;
        ScnEnvLgt_EnableFlag100((char*)model + 0x31C, bit);
        u32 check = REvtCam_IsField98Set();
        s32 field2C = FLD(s32, self, 0x2C);
        CREvtModel* obj = (CREvtModel*)self;
        if (check == 0) {
            obj->setVisible((field2C - 1) == 0 ? 1 : 0);
        } else {
            obj->setVisible((field2C != 2) ? 1 : 0);
        }
    }
}

void func_801728F8(void* self) {
    if (FLD(void*, self, 0x20) != 0) {
        Vec3 pos;
        pos.a[0] = lbl_eu_80667770;
        pos.a[1] = lbl_eu_80667770;
        pos.a[2] = lbl_eu_80667770;
        ((CREvtModelPose*)simGetLeafActData(FLD(void*, self, 0x20)))->translate = pos;
        Vec3 rot;
        rot.a[0] = lbl_eu_80667770;
        rot.a[1] = lbl_eu_80667770;
        rot.a[2] = lbl_eu_80667770;
        ((CREvtModelPose*)simGetLeafActData(FLD(void*, self, 0x20)))->rotate = rot;
    }
}

void releaseEvtAnim28(void* self) {
    void* p = FLD(void*, self, 0x28);
    if (p != 0) { Scn_IsAnimActiveOrNull(p); FLD(void*, self, 0x28) = 0; }
}

int isEvtDataFlagSet(void* self) {
    void* p = FLD(void*, self, 0x1C);
    if (p == 0) { return 0; }
    return (FLD(u32, p, 0x58) >> 4) & 1;
}

void func_801729F0(void* self, void* pData, void* pModelData) {
    u32 flags = FLD(u32, pModelData, 4);
    reinterpret_cast<CREvtModel*>(self)->onEvent(flags & 1);
    void* model = FLD(void*, self, 0x20);
    if (model == 0) return;
    simSetFlag2OnTree(model, FLD(u32, pModelData, 4) & 1);
    void* p24 = FLD(void*, self, 0x24);
    if (p24 != 0) {
        reinterpret_cast<CREvtModel*>(self)->vfunc_08();
        if (REvtCam_GetFieldA5Flag() == 0) { func_80172EA4(self, p24, model, 1); }
        FLD(void*, self, 0x28) = p24;
        FLD(void*, self, 0x24) = 0;
    }
    if (pData == 0) {
        void* p1C = FLD(void*, self, 0x1C);
        if (!(FLD(u32, p1C, 0x58) & 0x8)) {
            void* m = FLD(void*, self, 0x20);
            static_cast<CScnItemModel*>(m)->vfunc84(1);
        }
        return;
    }
    void* newData = Scn_InitGlobalA(&lbl_eu_80663E14, pData, 0);
    FLD(void*, self, 0x24) = newData;
    func_80482DF4(model, REvtCam_GetFieldA5Flag() == 0 ? 1 : 0);
    func_804839D4(model, (u32)newData, 0, 0, 0, 1, -1);
    func_80172CE4(self, model);
    static_cast<CScnItemModel*>(model)->vfunc84(0);
    simSetValue7E8(model, 1);
    FLD(u32, model, 0x7A4) |= 0x20000000;
    FLD(s32, self, 0x2C) = 0;
    FLD(s32, self, 0x30) = 0;
    { int tmp = 0; func_8016AF4C(newData, &lbl_eu_80503344[0], &tmp); FLD(s32, self, 0x2C) = tmp; }
    { int tmp = 0; func_8016AF4C(newData, &lbl_eu_80503344[0x0B], &tmp); FLD(s32, self, 0x30) = tmp; }
    u32 check = REvtCam_IsField98Set();
    CREvtModel* obj2 = (CREvtModel*)self;
    if (check == 0) {
        obj2->setVisible((FLD(s32, self, 0x2C) - 1) == 0 ? 1 : 0);
    } else {
        obj2->setVisible((FLD(s32, self, 0x2C) != 2) ? 1 : 0);
    }
    int sv = 0; func_8016AF4C(newData, &lbl_eu_80503344[0x13], &sv);
    if (sv >= 1) { func_80482B3C(model, 1); }
    int sv2 = 1; func_8016AF4C(newData, &lbl_eu_80503344[0x1E], &sv2);
    if (sv2 != 0) { FLD(u32, model, 0x7A0) |= 0x8; }
    else { FLD(u32, model, 0x7A0) &= ~0x8; }
    if (REvtCam_GetFieldA5Flag() == 0) { func_80172EA4(self, newData, model, 0); }
    int result = EvtSeqCheckRegionStatus(newData);
    static_cast<CScnItemModel*>(model)->vfunc84(result);
}

void emptyEvtModelHook(void) {}

void func_80172CC4(void* self, int visible) {
    CScnItemModel* model = (CScnItemModel*)FLD(void*, self, 0x20);
    if (model != 0) { model->vfuncB4(visible); }
}

void func_80172CE4(void* self, void* model) {
    const char* strBase = lbl_eu_80503344;
    int stackVal = 0;
    float stackF1 = lbl_eu_80667774;
    float stackF0 = lbl_eu_80667778;
    func_8016AF4C(FLD(void*, self, 0x24), &strBase[0x24], &stackVal);
    if (stackVal > 0) {
        stackVal--;
        EvtSeqFindResFloat(FLD(void*, self, 0x24), &strBase[0x2C], &stackF1);
        EvtSeqFindResFloat(FLD(void*, self, 0x24), &strBase[0x36], &stackF0);
    } else { stackVal = 1; }
    static_cast<CScnItemModel*>(model)->vfunc50(stackVal == 0 ? 0 : 1);
    void* shadow = __dynamic_cast(FLD(void*, model, 0x7EC), 0, (void*)&lbl_eu_806623F8, (void*)&lbl_eu_80662400, 0);
    if (shadow != 0) {
        if (stackVal == 1) { FLD(u8, shadow, 0x2A) = 1; }
        else if (stackVal == 2) { FLD(u8, shadow, 0x2A) = 0; }
        float f255 = lbl_eu_8066777C;
        int a = (int)(f255 * stackF0);
        FLD(u8, shadow, 4) = 0; FLD(u8, shadow, 5) = 0;
        FLD(u8, shadow, 6) = 0; FLD(u8, shadow, 7) = (u8)a;
        FLD(f32, shadow, 8) = stackF1;
    }
}

void func_80172EA4(void* self, void* animData, void* model, int flag) {
    nw4r::g3d::ResFile resFile((void*)((char*)animData + 0xC));
    int numAnmChr = resFile.GetResAnmChrNumEntries();
    for (int i = 0; i < numAnmChr; i++) {
        void* anmChrData = GetResAnmChr__Q34nw4r3g3d7ResFileCFi((char*)animData + 0xC, i);
        nw4r::g3d::ResAnmChr anmChr(anmChrData);
        void* userData = GetResUserData__Q34nw4r3g3d9ResAnmChrFv(&anmChr);
        if (userData == 0) continue;
        void* dic = (char*)userData + 4;
        if ((u32)dic & 3) { Panic__Q24nw4r2dbFPCciPCce(lbl_eu_80530F08, 0x54, lbl_eu_80530EE0); }
        void* result = __vc__Q34nw4r3g3d6ResDicCFPCc(dic, &lbl_eu_80503344[0x3D]);
        if ((u32)result & 3) { Panic__Q24nw4r2dbFPCciPCce(lbl_eu_80530D54, 0x26, lbl_eu_80530D2C); }
        if (result == 0) continue;
        if (FLD(u32, result, 0xC) != 2) { Panic__Q24nw4r2dbFPCciPCce(lbl_eu_80530ECC, 0x44, lbl_eu_80530E88); }
        void* dataPtr = 0;
        if (FLD(u32, result, 4) != 0) { dataPtr = (char*)result + FLD(u32, result, 4); }
        if (dataPtr != 0) {
            static_cast<CScnItemModel*>(model)->vfunc28((u32)dataPtr, flag);
        }
    }
    if (flag == 0) {
        void* p1C = FLD(void*, self, 0x1C);
        if (FLD(u32, p1C, 0x58) & 0x100) {
            static_cast<CScnItemModel*>(model)->vfunc28((u32)&lbl_eu_80503344[0x44], 0);
        }
    }
}

void* createEvtModelObj(void* self) {
    u32 type = FLD(u32, self, 0x28);
    if (type == 1) {
        void* obj = EvtObj_AllocBlock(0x1F0);
        if (obj != 0) { __ct__CREvtModelMap(obj, self); }
        return obj;
    } else if (type == 2) {
        if (FLD(s8, self, 0x16) != 0x30 || FLD(s8, self, 0x17) != 0x30) {
            void* obj = EvtObj_AllocBlock(0xB4);
            if (obj != 0) { __ct__CREvtModelPc(obj, self); }
            return obj;
        }
    }
    void* obj = EvtObj_AllocBlock(0x88);
    if (obj != 0) { __ct__CREvtModelObj(obj, self); }
    return obj;
}

int isEvtFlagBit0Set(void* self) {
    return FLD(u32, self, 0x18) & 1;
}

u32 isEvtFlagBit4Set(void* self) {
    return (FLD(u32, self, 0x18) >> 4) & 1;
}

int zeroEvtModelHook(void* self) {
    (void)self; return 0;
}

} // extern "C"
// Typified wave7: retail .data as stand-in typed pointer tables
// (MI vtables/RTTI referencing other-TU methods; TU emits no compiler
// .data of its own, so these three objects ARE the section, in order).
extern "C" {
extern void* __RTTI__Q22cf13CfObjectActor;
extern const void* lbl_eu_80662408[2];  // defined with the .sdata below
extern void getField20(), CREvtObjIsBusyDefault(), CREvtObjVfunc24Default();  // cross-TU slots of the 0x44 table
extern void CActorState_setObjStateBit3__Q22cf11CActorStateFv(), CBattleState_getOwner__Q22cf13CfObjectActorFv(), CBattleState_callOwnerObject__Q22cf13CfObjectActorFv(), CBattleState_getParamPtrName__Q22cf13CfObjectActorFv(), CBattleState_UnkVirtualFunc4__Q22cf12CBattleStateFv(), CBattleState_enterStatusEntry__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry();
extern void CBattleState_addStagingEntry__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry(), CBattleState_clearStatusId__Q22cf12CBattleStateFv(), CBattleState_applyEventEntry__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry(), CBattleState_UnkVirtualFunc9__Q22cf12CBattleStateFv(), CBattleState_removeKeyedEntries__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry(), CBattleState_clearEntriesByMask__Q22cf12CBattleStateFUl();
extern void CBattleState_UnkVirtualFunc12__Q22cf12CBattleStateFv(), CBattleState_getStatusSlot__Q22cf12CBattleStateFi(), CBattleState_UnkVirtualFunc14__Q22cf12CBattleStateFv(), CBattleState_UnkVirtualFunc15__Q22cf12CBattleStateFv(), CBattleState_UnkVirtualFunc16__Q22cf12CBattleStateFv(), CBattleState_notifyEntryUpdated__Q22cf13CfObjectActorFv();
extern void CBattleState_getLinkedActorId__Q22cf13CfObjectActorFv(), CBattleState_UnkVirtualFunc19__Q22cf12CBattleStateFv(), CBattleState_getEntryByIndex__Q22cf12CBattleStateFv(), CBattleState_fetchStatusEntry__Q22cf12CBattleStateFv(), CBattleState_getEventEntry__Q22cf12CBattleStateFv(), CBattleState_getStatusEntry__Q22cf12CBattleStateFv();
extern void CBattleState_copyStatusEntries__Q22cf12CBattleStateFv(), CBattleState_getEntryArray__Q22cf12CBattleStateFv(), CBattleState_applyArtsTable__Q22cf12CBattleStateFPCQ22cf20CBattleStateSrcEntry(), CBattleState_getStatusBlock__Q22cf12CBattleStateFv(), CBattleState_getStagingRecord__Q22cf12CBattleStateFv(), CBattleState_clearStagingEntries__Q22cf12CBattleStateFv();
extern void CBattleState_UnkVirtualFunc30__Q22cf12CBattleStateFv(), CBattleState_getEventMask__Q22cf12CBattleStateFUl(), CBattleState_UnkVirtualFunc32__Q22cf12CBattleStateFv(), CBattleState_getStatusMask__Q22cf12CBattleStateFUl(), CActorParam_UnkVirtualFunc1__Q22cf13CfObjectActorFv(), CActorParam_UnkVirtualFunc2__Q22cf13CfObjectActorFv();
extern void CActorParam_UnkVirtualFunc3__Q22cf13CfObjectActorFv(), CActorParam_resetArtsStatus__Q22cf11CActorParamFv(), CActorParam_commitArtsStatus__Q22cf11CActorParamFv(), CActorParam_resetArtsStatus__Q22cf13CfObjectActorFv(), CActorParam_setupActorState__Q22cf11CActorParamFv(), CActorParam_resetArtsMatrix__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc9__Q22cf11CActorParamFv(), CActorParam_applyTargetEvent__Q22cf11CActorParamFv(), CActorParam_applyStatusHeal__Q22cf11CActorParamFv(), CActorParam_applyEventStatus__Q22cf11CActorParamFv(), CActorParam_resetGaugesAndRefresh__Q22cf11CActorParamFv(), CActorParam_setStatByte__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc15__Q22cf11CActorParamFv(), CActorParam_setStatScale__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc17__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc18__Q22cf11CActorParamFv(), CActorParam_getActorType__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc20__Q22cf11CActorParamFv();
extern void CActorParam_setScale__Q22cf13CfObjectActorFv(), CActorParam_UnkVirtualFunc22__Q22cf11CActorParamFv(), CActorParam_getScale__Q22cf13CfObjectActorFv(), CActorParam_UnkVirtualFunc24__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc25__Q22cf11CActorParamFv(), CActorParam_getArtsLevel__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc27__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc28__Q22cf11CActorParamFv(), CActorParam_getActorLevel__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc30__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc31__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc32__Q22cf11CActorParamFv();
extern void CActorParam_setHp__Q22cf13CfObjectActorFv(), CActorParam_addHp__Q22cf13CfObjectActorFv(), CActorParam_applyDamage__Q22cf13CfObjectActorFv(), CActorParam_UnkVirtualFunc36__Q22cf11CActorParamFv(), CActorParam_getHp__Q22cf11CActorParamFv(), CActorParam_getDamageScale__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc39__Q22cf11CActorParamFv(), CActorParam_applyStoredHp__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc41__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc42__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc43__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc44__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc45__Q22cf11CActorParamFv(), CActorParam_applyStoredHitCharge__Q22cf11CActorParamFv(), CActorParam_setTensionPoints__Q22cf11CActorParamFv(), CActorParam_addArtsGauge__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc49__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc50__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc51__Q22cf11CActorParamFv(), CActorParam_applyStoredArtsGauge__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc53__Q22cf11CActorParamFv(), CActorParam_addGauge__Q22cf13CfObjectActorFv(), CActorParam_setGaugeFlagA__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc56__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc57__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc58__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc59__Q22cf11CActorParamFv(), CActorParam_addSecondGauge__Q22cf13CfObjectActorFv(), CActorParam_setGaugeFlagB__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc62__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc63__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc64__Q22cf11CActorParamFv(), CActorParam_setGaugeRateA__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc66__Q22cf11CActorParamFv(), CActorParam_resetGaugeBattleEntry__Q22cf11CActorParamFv(), CActorParam_setGaugeRateB__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc69__Q22cf11CActorParamFv(), CActorParam_resetSecondGaugeBattleEntry__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc71__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc72__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc73__Q22cf11CActorParamFv(), CActorParam_setMoveRate__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc75__Q22cf11CActorParamFv(), CActorParam_getMoveRate__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc77__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc78__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc79__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc80__Q22cf11CActorParamFv();
extern void CActorParam_setSpentCurrency__Q22cf11CActorParamFv(), CActorParam_addSpentCurrency__Q22cf11CActorParamFv(), CActorParam_addAccumCurrency__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc84__Q22cf11CActorParamFv(), CActorParam_getSpentCurrency__Q22cf11CActorParamFv(), CActorParam_getCurrencyBalance__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc87__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc88__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc89__Q22cf11CActorParamFv(), CActorParam_addSecondCurrency__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc91__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc92__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc93__Q22cf11CActorParamFv(), CActorParam_getArtsDataBlock__Q22cf11CActorParamFv(), CActorParam_getStatusSyncBlock__Q22cf11CActorParamFv(), CActorParam_getArtsSyncBlock__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc97__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc98__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc99__Q22cf11CActorParamFv(), CActorParam_getBattleParams__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc101__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc102__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc103__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc104__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc105__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc106__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc107__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc108__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc109__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc110__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc111__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc112__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc113__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc114__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc115__Q22cf11CActorParamFv(), CActorParam_setGaugeFloat__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc117__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc118__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc119__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc120__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc121__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc122__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc123__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc124__Q22cf11CActorParamFv(), CActorParam_getArtsSlotIds__Q22cf11CActorParamFv(), CActorParam_getCopyParam__Q22cf11CActorParamFv(), CActorParam_getStatusTable__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc128__Q22cf11CActorParamFv();
extern void CActorParam_getBattleStats__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc130__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc131__Q22cf11CActorParamFv(), CActorParam_getMoveRecord__Q22cf11CActorParamFv(), CActorParam_getStatsSlot__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc134__Q22cf11CActorParamFv();
extern void CActorParam_reinitBattleStats__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc136__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc137__Q22cf11CActorParamFv(), CActorParam_isBattleLocked__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc139__Q22cf11CActorParamFv(), CActorParam_addHate__Q22cf13CfObjectActorFv();
extern void CActorParam_resetGaugeEntry__Q22cf11CActorParamFv(), CActorParam_resetAllGaugeEntries__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc143__Q22cf11CActorParamFv(), CActorParam_findGaugeEntry__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc145__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc146__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc147__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc148__Q22cf11CActorParamFv(), CActorParam_findMaxGaugeEntry__Q22cf11CActorParamFv(), CActorParam_findMinGaugeEntry__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc151__Q22cf11CActorParamFv(), CActorParam_getTensionStatus__Q22cf11CActorParamFv();
extern void CActorParam_accumulateTension__Q22cf11CActorParamFv(), CActorParam_raiseTensionValue__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc155__Q22cf11CActorParamFv(), CActorParam_resetTensionState__Q22cf11CActorParamFv(), CActorParam_getStatusCount__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc158__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc159__Q22cf11CActorParamFv(), CActorParam_initStatusCounter__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc161__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc162__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc163__Q22cf11CActorParamFv(), CActorParam_getActorModeFlags__Q22cf11CActorParamFv();
extern void CActorParam_getBattleHitFlags__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc166__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc167__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc168__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc169__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc170__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc171__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc172__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc173__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc174__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc175__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc176__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc177__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc178__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc179__Q22cf13CfObjectActorFv(), CActorParam_UnkVirtualFunc180__Q22cf13CfObjectActorFv(), CActorParam_UnkVirtualFunc181__Q22cf11CActorParamFv(), CAIAction_UnkVirtualFunc1__Q22cf9CAIActionFv();
extern void CAIAction_UnkVirtualFunc2__Q22cf9CAIActionFv(), CObjectState_setStateBitMask__Q22cf12CObjectStateFv(), CObjectState_checkStateFlags__Q22cf12CObjectStateFv(), CObjectState_setStateBitFlag__Q22cf12CObjectStateFv(), CObjectState_clearStateWord4__Q22cf12CObjectStateFv(), CObjectState_applyStateFlags__Q22cf12CObjectStateFv();
extern void CObjectState_clearStateWord8__Q22cf12CObjectStateFv(), CObjectState_clearStateFlags8__Q22cf12CObjectStateFv(), CObjectState_UnkVirtualFunc8__Q22cf12CObjectStateFv(), CObjectState_UnkVirtualFunc9__Q22cf12CObjectStateFv(), CObjectState_setStateBitMask0__Q22cf12CObjectStateFv(), CObjectState_getStateData__Q22cf12CObjectStateFv();
extern void CObjectState_setStateBitMask2__Q22cf12CObjectStateFv(), CObjectState_setStateBitMask3__Q22cf12CObjectStateFv(), CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv(), CObjectParam_getParamPtr__Q22cf13CfObjectActorFv(), CObjectParam_hasObjectName__Q22cf12CObjectParamFv(), CObjectParam_UnkVirtualFunc4__Q22cf12CObjectParamFv();
extern void CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv(), CObjectParam_signalActionEnd__Q22cf12CObjectParamFv(), destroyActorParam__Q22cf13CfObjectActorFv(), CfObject_UnkVirtualFunc2__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv(), CfObject_UnkVirtualFunc4__Q22cf12CfObjectMoveFv();
extern void CfObject_UnkVirtualFunc5__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc6__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc7__Q22cf12CfObjectMoveFv(), CfObject_notifyEventDone__Q22cf13CfObjectModelFv(), CfObject_isMoveActiveNow__Q22cf12CfObjectMoveFv(), CfObject_forwardSubObject__Q22cf12CfObjectMoveFv();
extern void CfObject_UnkVirtualFunc11__Q22cf8CfObjectFv(), CfObject_UnkVirtualFunc12__Q22cf12CfObjectMoveFv(), CfObject_queryTargetState__Q22cf12CfObjectMoveFv(), CfObject_pushRefreshValue__Q22cf13CfObjectActorFf(), CfObject_getMoveSpeedRate__Q22cf12CfObjectMoveFv(), CfObject_pushRefreshExtra__Q22cf12CfObjectMoveFv();
extern void CfObject_readRefreshValue__Q22cf12CfObjectMoveFv(), CfObject_checkTargetState__Q22cf12CfObjectMoveFv(), CfObject_setMoveTargetVec__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc20__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv(), CfObject_UnkVirtualFunc22__Q22cf12CfObjectMoveFv();
extern void CfObject_getPosVector__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc24__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc25__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc26__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc27__Q22cf12CfObjectMoveFPv(), CfObject_UnkVirtualFunc28__Q22cf13CfObjectModelFv();
extern void CfObject_setMoveHeadAngle__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc30__Q22cf12CfObjectMoveFv(), CfObject_getMoveHeadAngle__Q22cf13CfObjectActorFv(), CfObject_UnkVirtualFunc32__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc33__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc34__Q22cf13CfObjectModelFv();
extern void CfObject_UnkVirtualFunc35__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc36__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc37__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc38__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc39__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc40__Q22cf12CfObjectMoveFv();
extern void CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv(), CfObject_UnkVirtualFunc42__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc43__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv(), CfObject_UnkVirtualFunc45__Q22cf12CfObjectMoveFv(), CfObject_setMoveTargetPtr__Q22cf12CfObjectMoveFv();
extern void CfObject_createMoveTarget__Q22cf12CfObjectMoveFv(), CfObject_getCurrentTarget__Q22cf12CfObjectMoveFv(), CfObject_forwardNpcAction__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc50__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc51__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc52__Q22cf13CfObjectModelFv();
extern void CfObject_UnkVirtualFunc53__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc54__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc55__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc56__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc57__Q22cf12CfObjectMoveFv(), CfObject_getMoveRateScale__Q22cf12CfObjectMoveFv();
extern void CfObject_UnkVirtualFunc59__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc60__Q22cf12CfObjectMoveFv(), CfObject_setAnimSlotEntry__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc62__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc63__Q22cf13CfObjectModelFv(), CfObject_setMoveBusyState__Q22cf12CfObjectMoveFi();
extern void CfObject_setMoveReadyFlag__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc66__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc67__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc69__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc70__Q22cf12CfObjectMoveFv();
extern void CfObject_UnkVirtualFunc71__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc72__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc73__Q22cf13CfObjectModelFv(), CfObjectModel_UnkVirtualFunc1__Q22cf12CfObjectMoveFv(), CfObjectModel_UnkVirtualFunc2__Q22cf12CfObjectMoveFv(), CfObjectModel_UnkVirtualFunc3__Q22cf13CfObjectModelFv();
extern void CfObjectModel_UnkVirtualFunc4__Q22cf12CfObjectMoveFv(), CfObjectModel_UnkVirtualFunc5__Q22cf12CfObjectMoveFv(), CfObjectModel_UnkVirtualFunc6__Q22cf12CfObjectMoveFv(), CfObjectModel_UnkVirtualFunc7__Q22cf13CfObjectModelFv(), CfObjectModel_UnkVirtualFunc8__Q22cf13CfObjectModelFv(), CfObjectModel_UnkVirtualFunc9__Q22cf13CfObjectModelFv();
extern void CfObjectModel_UnkVirtualFunc10__Q22cf13CfObjectModelFv(), CfObjectModel_UnkVirtualFunc11__Q22cf13CfObjectModelFv(), CfObjectModel_UnkVirtualFunc12__Q22cf13CfObjectModelFv(), CfObjectModel_UnkVirtualFunc13__Q22cf13CfObjectModelFv(), CfObjectModel_UnkVirtualFunc14__Q22cf13CfObjectModelFv(), CfObjectModel_UnkVirtualFunc15__Q22cf13CfObjectModelFv();
extern void CfObjectModel_UnkVirtualFunc16__Q22cf13CfObjectModelFv(), CfObjectModel_UnkVirtualFunc17__Q22cf13CfObjectModelFv(), CfObjectModel_UnkVirtualFunc18__Q22cf12CfObjectMoveFv(), CfObjectModel_UnkVirtualFunc19__Q22cf12CfObjectMoveFv(), CfObjectModel_UnkVirtualFunc20__Q22cf13CfObjectModelFv(), CfObjectMove_dropMoveRequest__Q22cf12CfObjectMoveFv();
extern void CfObjectMove_detachModelList__Q22cf12CfObjectMoveFv(), CfObjectMove_releaseSlotById__Q22cf12CfObjectMoveFv(), CfObjectMove_recordMoveValue__Q22cf12CfObjectMoveFv(), CfObjectMove_getMovementRate__Q22cf12CfObjectMoveFv(), CfObjectMove_storeActionSrcId__Q22cf13CfObjectActorFv(), CfObjectMove_refreshNpcState__Q22cf12CfObjectMoveFv();
extern void CfObjectMove_updateNpcTarget__Q22cf12CfObjectMoveFv(), CfObjectMove_queryNpcAdvance__Q22cf12CfObjectMoveFv(), CfObjectMove_UnkVirtualFunc10__Q22cf12CfObjectMoveFv(), CfObjectMove_writeTargetField__Q22cf12CfObjectMoveFv(), CfObjectMove_advanceNpcTarget__Q22cf12CfObjectMoveFv(), CfObjectMove_commandNpcTarget__Q22cf12CfObjectMoveFv();
extern void CfObjectMove_restartNpcTarget__Q22cf12CfObjectMoveFv(), CfObjectMove_hasActorScale__Q22cf13CfObjectActorFv(), CfObjectMove_attachEffectSlot__Q22cf12CfObjectMoveFv(), CfObjectMove_setEffectSlotBit__Q22cf12CfObjectMoveFv(), CfObjectMove_transferSlotBits__Q22cf12CfObjectMoveFv(), CfObjectMove_isEffectSlotUsed__Q22cf12CfObjectMoveFv();
extern void CfObjectMove_signalEffectSlot__Q22cf12CfObjectMoveFv(), CfObjectMove_flushEffectSlots__Q22cf12CfObjectMoveFv(), CfObjectMove_setAllEffectBits__Q22cf12CfObjectMoveFv(), CfObjectMove_loadResourceById__Q22cf12CfObjectMoveFv(), __dt__Q22cf13CfObjectActorFv(), CfObjectActor_hasActorScale__Q22cf13CfObjectActorFv();
extern void CfObjectActor_clearStatusPair__Q22cf13CfObjectActorFv(), CfObjectActor_flushStatusPair__Q22cf13CfObjectActorFv(), CfObjectActor_takeDamageValue__Q22cf13CfObjectActorFv(), CfObjectActor_readFacingAngle__Q22cf13CfObjectActorFv(), CfObjectActor_getAdjustedFacing__Q22cf13CfObjectActorFv(), CfObjectActor_readActionScale__Q22cf13CfObjectActorFv();
extern void CfObjectActor_sharesMoveFlags__Q22cf13CfObjectActorFv(), CfObjectActor_pushRefreshValue__Q22cf13CfObjectActorFv(), CfObjectActor_storeActionSrcId__Q22cf13CfObjectActorFv(), CfObjectActor_getActionSrcId__Q22cf13CfObjectActorFv(), CfObjectActor_clearActionSrcId__Q22cf13CfObjectActorFv();

// .data 0x5D4: concatenated MI vtables (RTTI + this-module offset headers).
__declspec(section ".data") __attribute__((used))
const void* lbl_eu_8053109C[373] = {
    // +0x0 CActorState secondary (RTTI +0x0)
    &__RTTI__Q22cf13CfObjectActor, 0, (void*)CActorState_setObjStateBit3__Q22cf11CActorStateFv, &__RTTI__Q22cf13CfObjectActor, // +0x0
    (void*)0xFFFFFFF8, (void*)CBattleState_getOwner__Q22cf13CfObjectActorFv, (void*)CBattleState_callOwnerObject__Q22cf13CfObjectActorFv, (void*)CBattleState_getParamPtrName__Q22cf13CfObjectActorFv, // +0x10
    (void*)CBattleState_UnkVirtualFunc4__Q22cf12CBattleStateFv, (void*)CBattleState_enterStatusEntry__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry, (void*)CBattleState_addStagingEntry__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry, (void*)CBattleState_clearStatusId__Q22cf12CBattleStateFv, // +0x20
    (void*)CBattleState_applyEventEntry__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry, (void*)CBattleState_UnkVirtualFunc9__Q22cf12CBattleStateFv, (void*)CBattleState_removeKeyedEntries__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry, (void*)CBattleState_clearEntriesByMask__Q22cf12CBattleStateFUl, // +0x30
    (void*)CBattleState_UnkVirtualFunc12__Q22cf12CBattleStateFv, (void*)CBattleState_getStatusSlot__Q22cf12CBattleStateFi, (void*)CBattleState_UnkVirtualFunc14__Q22cf12CBattleStateFv, (void*)CBattleState_UnkVirtualFunc15__Q22cf12CBattleStateFv, // +0x40
    (void*)CBattleState_UnkVirtualFunc16__Q22cf12CBattleStateFv, (void*)CBattleState_notifyEntryUpdated__Q22cf13CfObjectActorFv, (void*)CBattleState_getLinkedActorId__Q22cf13CfObjectActorFv, (void*)CBattleState_UnkVirtualFunc19__Q22cf12CBattleStateFv, // +0x50
    (void*)CBattleState_getEntryByIndex__Q22cf12CBattleStateFv, (void*)CBattleState_fetchStatusEntry__Q22cf12CBattleStateFv, (void*)CBattleState_getEventEntry__Q22cf12CBattleStateFv, (void*)CBattleState_getStatusEntry__Q22cf12CBattleStateFv, // +0x60
    (void*)CBattleState_copyStatusEntries__Q22cf12CBattleStateFv, (void*)CBattleState_getEntryArray__Q22cf12CBattleStateFv, (void*)CBattleState_applyArtsTable__Q22cf12CBattleStateFPCQ22cf20CBattleStateSrcEntry, (void*)CBattleState_getStatusBlock__Q22cf12CBattleStateFv, // +0x70
    (void*)CBattleState_getStagingRecord__Q22cf12CBattleStateFv, (void*)CBattleState_clearStagingEntries__Q22cf12CBattleStateFv, (void*)CBattleState_UnkVirtualFunc30__Q22cf12CBattleStateFv, (void*)CBattleState_getEventMask__Q22cf12CBattleStateFUl, // +0x80
    (void*)CBattleState_UnkVirtualFunc32__Q22cf12CBattleStateFv, (void*)CBattleState_getStatusMask__Q22cf12CBattleStateFUl, (void*)CActorParam_UnkVirtualFunc1__Q22cf13CfObjectActorFv, (void*)CActorParam_UnkVirtualFunc2__Q22cf13CfObjectActorFv, // +0x90
    (void*)CActorParam_UnkVirtualFunc3__Q22cf13CfObjectActorFv, (void*)CActorParam_resetArtsStatus__Q22cf11CActorParamFv, (void*)CActorParam_commitArtsStatus__Q22cf11CActorParamFv, (void*)CActorParam_resetArtsStatus__Q22cf13CfObjectActorFv, // +0xA0
    (void*)CActorParam_setupActorState__Q22cf11CActorParamFv, (void*)CActorParam_resetArtsMatrix__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc9__Q22cf11CActorParamFv, (void*)CActorParam_applyTargetEvent__Q22cf11CActorParamFv, // +0xB0
    (void*)CActorParam_applyStatusHeal__Q22cf11CActorParamFv, (void*)CActorParam_applyEventStatus__Q22cf11CActorParamFv, (void*)CActorParam_resetGaugesAndRefresh__Q22cf11CActorParamFv, (void*)CActorParam_setStatByte__Q22cf11CActorParamFv, // +0xC0
    (void*)CActorParam_UnkVirtualFunc15__Q22cf11CActorParamFv, (void*)CActorParam_setStatScale__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc17__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc18__Q22cf11CActorParamFv, // +0xD0
    (void*)CActorParam_getActorType__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc20__Q22cf11CActorParamFv, (void*)CActorParam_setScale__Q22cf13CfObjectActorFv, (void*)CActorParam_UnkVirtualFunc22__Q22cf11CActorParamFv, // +0xE0
    (void*)CActorParam_getScale__Q22cf13CfObjectActorFv, (void*)CActorParam_UnkVirtualFunc24__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc25__Q22cf11CActorParamFv, (void*)CActorParam_getArtsLevel__Q22cf11CActorParamFv, // +0xF0
    (void*)CActorParam_UnkVirtualFunc27__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc28__Q22cf11CActorParamFv, (void*)CActorParam_getActorLevel__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc30__Q22cf11CActorParamFv, // +0x100
    (void*)CActorParam_UnkVirtualFunc31__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc32__Q22cf11CActorParamFv, (void*)CActorParam_setHp__Q22cf13CfObjectActorFv, (void*)CActorParam_addHp__Q22cf13CfObjectActorFv, // +0x110
    (void*)CActorParam_applyDamage__Q22cf13CfObjectActorFv, (void*)CActorParam_UnkVirtualFunc36__Q22cf11CActorParamFv, (void*)CActorParam_getHp__Q22cf11CActorParamFv, (void*)CActorParam_getDamageScale__Q22cf11CActorParamFv, // +0x120
    (void*)CActorParam_UnkVirtualFunc39__Q22cf11CActorParamFv, (void*)CActorParam_applyStoredHp__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc41__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc42__Q22cf11CActorParamFv, // +0x130
    (void*)CActorParam_UnkVirtualFunc43__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc44__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc45__Q22cf11CActorParamFv, (void*)CActorParam_applyStoredHitCharge__Q22cf11CActorParamFv, // +0x140
    (void*)CActorParam_setTensionPoints__Q22cf11CActorParamFv, (void*)CActorParam_addArtsGauge__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc49__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc50__Q22cf11CActorParamFv, // +0x150
    (void*)CActorParam_UnkVirtualFunc51__Q22cf11CActorParamFv, (void*)CActorParam_applyStoredArtsGauge__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc53__Q22cf11CActorParamFv, (void*)CActorParam_addGauge__Q22cf13CfObjectActorFv, // +0x160
    (void*)CActorParam_setGaugeFlagA__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc56__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc57__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc58__Q22cf11CActorParamFv, // +0x170
    (void*)CActorParam_UnkVirtualFunc59__Q22cf11CActorParamFv, (void*)CActorParam_addSecondGauge__Q22cf13CfObjectActorFv, (void*)CActorParam_setGaugeFlagB__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc62__Q22cf11CActorParamFv, // +0x180
    (void*)CActorParam_UnkVirtualFunc63__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc64__Q22cf11CActorParamFv, (void*)CActorParam_setGaugeRateA__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc66__Q22cf11CActorParamFv, // +0x190
    (void*)CActorParam_resetGaugeBattleEntry__Q22cf11CActorParamFv, (void*)CActorParam_setGaugeRateB__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc69__Q22cf11CActorParamFv, (void*)CActorParam_resetSecondGaugeBattleEntry__Q22cf11CActorParamFv, // +0x1A0
    (void*)CActorParam_UnkVirtualFunc71__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc72__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc73__Q22cf11CActorParamFv, (void*)CActorParam_setMoveRate__Q22cf11CActorParamFv, // +0x1B0
    (void*)CActorParam_UnkVirtualFunc75__Q22cf11CActorParamFv, (void*)CActorParam_getMoveRate__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc77__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc78__Q22cf11CActorParamFv, // +0x1C0
    (void*)CActorParam_UnkVirtualFunc79__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc80__Q22cf11CActorParamFv, (void*)CActorParam_setSpentCurrency__Q22cf11CActorParamFv, (void*)CActorParam_addSpentCurrency__Q22cf11CActorParamFv, // +0x1D0
    (void*)CActorParam_addAccumCurrency__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc84__Q22cf11CActorParamFv, (void*)CActorParam_getSpentCurrency__Q22cf11CActorParamFv, (void*)CActorParam_getCurrencyBalance__Q22cf11CActorParamFv, // +0x1E0
    (void*)CActorParam_UnkVirtualFunc87__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc88__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc89__Q22cf11CActorParamFv, (void*)CActorParam_addSecondCurrency__Q22cf11CActorParamFv, // +0x1F0
    (void*)CActorParam_UnkVirtualFunc91__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc92__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc93__Q22cf11CActorParamFv, (void*)CActorParam_getArtsDataBlock__Q22cf11CActorParamFv, // +0x200
    (void*)CActorParam_getStatusSyncBlock__Q22cf11CActorParamFv, (void*)CActorParam_getArtsSyncBlock__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc97__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc98__Q22cf11CActorParamFv, // +0x210
    (void*)CActorParam_UnkVirtualFunc99__Q22cf11CActorParamFv, (void*)CActorParam_getBattleParams__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc101__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc102__Q22cf11CActorParamFv, // +0x220
    (void*)CActorParam_UnkVirtualFunc103__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc104__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc105__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc106__Q22cf11CActorParamFv, // +0x230
    (void*)CActorParam_UnkVirtualFunc107__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc108__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc109__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc110__Q22cf11CActorParamFv, // +0x240
    (void*)CActorParam_UnkVirtualFunc111__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc112__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc113__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc114__Q22cf11CActorParamFv, // +0x250
    (void*)CActorParam_UnkVirtualFunc115__Q22cf11CActorParamFv, (void*)CActorParam_setGaugeFloat__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc117__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc118__Q22cf11CActorParamFv, // +0x260
    (void*)CActorParam_UnkVirtualFunc119__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc120__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc121__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc122__Q22cf11CActorParamFv, // +0x270
    (void*)CActorParam_UnkVirtualFunc123__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc124__Q22cf11CActorParamFv, (void*)CActorParam_getArtsSlotIds__Q22cf11CActorParamFv, (void*)CActorParam_getCopyParam__Q22cf11CActorParamFv, // +0x280
    (void*)CActorParam_getStatusTable__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc128__Q22cf11CActorParamFv, (void*)CActorParam_getBattleStats__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc130__Q22cf11CActorParamFv, // +0x290
    (void*)CActorParam_UnkVirtualFunc131__Q22cf11CActorParamFv, (void*)CActorParam_getMoveRecord__Q22cf11CActorParamFv, (void*)CActorParam_getStatsSlot__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc134__Q22cf11CActorParamFv, // +0x2A0
    (void*)CActorParam_reinitBattleStats__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc136__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc137__Q22cf11CActorParamFv, (void*)CActorParam_isBattleLocked__Q22cf11CActorParamFv, // +0x2B0
    (void*)CActorParam_UnkVirtualFunc139__Q22cf11CActorParamFv, (void*)CActorParam_addHate__Q22cf13CfObjectActorFv, (void*)CActorParam_resetGaugeEntry__Q22cf11CActorParamFv, (void*)CActorParam_resetAllGaugeEntries__Q22cf11CActorParamFv, // +0x2C0
    (void*)CActorParam_UnkVirtualFunc143__Q22cf11CActorParamFv, (void*)CActorParam_findGaugeEntry__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc145__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc146__Q22cf11CActorParamFv, // +0x2D0
    (void*)CActorParam_UnkVirtualFunc147__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc148__Q22cf11CActorParamFv, (void*)CActorParam_findMaxGaugeEntry__Q22cf11CActorParamFv, (void*)CActorParam_findMinGaugeEntry__Q22cf11CActorParamFv, // +0x2E0
    (void*)CActorParam_UnkVirtualFunc151__Q22cf11CActorParamFv, (void*)CActorParam_getTensionStatus__Q22cf11CActorParamFv, (void*)CActorParam_accumulateTension__Q22cf11CActorParamFv, (void*)CActorParam_raiseTensionValue__Q22cf11CActorParamFv, // +0x2F0
    (void*)CActorParam_UnkVirtualFunc155__Q22cf11CActorParamFv, (void*)CActorParam_resetTensionState__Q22cf11CActorParamFv, (void*)CActorParam_getStatusCount__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc158__Q22cf11CActorParamFv, // +0x300
    (void*)CActorParam_UnkVirtualFunc159__Q22cf11CActorParamFv, (void*)CActorParam_initStatusCounter__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc161__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc162__Q22cf11CActorParamFv, // +0x310
    (void*)CActorParam_UnkVirtualFunc163__Q22cf11CActorParamFv, (void*)CActorParam_getActorModeFlags__Q22cf11CActorParamFv, (void*)CActorParam_getBattleHitFlags__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc166__Q22cf11CActorParamFv, // +0x320
    (void*)CActorParam_UnkVirtualFunc167__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc168__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc169__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc170__Q22cf11CActorParamFv, // +0x330
    (void*)CActorParam_UnkVirtualFunc171__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc172__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc173__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc174__Q22cf11CActorParamFv, // +0x340
    (void*)CActorParam_UnkVirtualFunc175__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc176__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc177__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc178__Q22cf11CActorParamFv, // +0x350
    (void*)CActorParam_UnkVirtualFunc179__Q22cf13CfObjectActorFv, (void*)CActorParam_UnkVirtualFunc180__Q22cf13CfObjectActorFv, (void*)CActorParam_UnkVirtualFunc181__Q22cf11CActorParamFv, &__RTTI__Q22cf13CfObjectActor, // +0x360
    (void*)0xFFFFCC80, (void*)CAIAction_UnkVirtualFunc1__Q22cf9CAIActionFv, (void*)CAIAction_UnkVirtualFunc2__Q22cf9CAIActionFv, &__RTTI__Q22cf13CfObjectActor, // +0x370
    (void*)0xFFFFC164, (void*)CObjectState_setStateBitMask__Q22cf12CObjectStateFv, (void*)CObjectState_checkStateFlags__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitFlag__Q22cf12CObjectStateFv, // +0x380
    (void*)CObjectState_clearStateWord4__Q22cf12CObjectStateFv, (void*)CObjectState_applyStateFlags__Q22cf12CObjectStateFv, (void*)CObjectState_clearStateWord8__Q22cf12CObjectStateFv, (void*)CObjectState_clearStateFlags8__Q22cf12CObjectStateFv, // +0x390
    (void*)CObjectState_UnkVirtualFunc8__Q22cf12CObjectStateFv, (void*)CObjectState_UnkVirtualFunc9__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitMask0__Q22cf12CObjectStateFv, (void*)CObjectState_getStateData__Q22cf12CObjectStateFv, // +0x3A0
    (void*)CObjectState_setStateBitMask2__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitMask3__Q22cf12CObjectStateFv, (void*)CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv, (void*)CObjectParam_getParamPtr__Q22cf13CfObjectActorFv, // +0x3B0
    (void*)CObjectParam_hasObjectName__Q22cf12CObjectParamFv, (void*)CObjectParam_UnkVirtualFunc4__Q22cf12CObjectParamFv, (void*)CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv, (void*)CObjectParam_signalActionEnd__Q22cf12CObjectParamFv, // +0x3C0
    (void*)destroyActorParam__Q22cf13CfObjectActorFv, (void*)CfObject_UnkVirtualFunc2__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc4__Q22cf12CfObjectMoveFv, // +0x3D0
    (void*)CfObject_UnkVirtualFunc5__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc6__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc7__Q22cf12CfObjectMoveFv, (void*)CfObject_notifyEventDone__Q22cf13CfObjectModelFv, // +0x3E0
    (void*)CfObject_isMoveActiveNow__Q22cf12CfObjectMoveFv, (void*)CfObject_forwardSubObject__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc11__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc12__Q22cf12CfObjectMoveFv, // +0x3F0
    (void*)CfObject_queryTargetState__Q22cf12CfObjectMoveFv, (void*)CfObject_pushRefreshValue__Q22cf13CfObjectActorFf, (void*)CfObject_getMoveSpeedRate__Q22cf12CfObjectMoveFv, (void*)CfObject_pushRefreshExtra__Q22cf12CfObjectMoveFv, // +0x400
    (void*)CfObject_readRefreshValue__Q22cf12CfObjectMoveFv, (void*)CfObject_checkTargetState__Q22cf12CfObjectMoveFv, (void*)CfObject_setMoveTargetVec__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc20__Q22cf13CfObjectModelFv, // +0x410
    (void*)CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc22__Q22cf12CfObjectMoveFv, (void*)CfObject_getPosVector__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc24__Q22cf13CfObjectModelFv, // +0x420
    (void*)CfObject_UnkVirtualFunc25__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc26__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc27__Q22cf12CfObjectMoveFPv, (void*)CfObject_UnkVirtualFunc28__Q22cf13CfObjectModelFv, // +0x430
    (void*)CfObject_setMoveHeadAngle__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc30__Q22cf12CfObjectMoveFv, (void*)CfObject_getMoveHeadAngle__Q22cf13CfObjectActorFv, (void*)CfObject_UnkVirtualFunc32__Q22cf12CfObjectMoveFv, // +0x440
    (void*)CfObject_UnkVirtualFunc33__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc34__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc35__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc36__Q22cf13CfObjectModelFv, // +0x450
    (void*)CfObject_UnkVirtualFunc37__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc38__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc39__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc40__Q22cf12CfObjectMoveFv, // +0x460
    (void*)CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc42__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc43__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv, // +0x470
    (void*)CfObject_UnkVirtualFunc45__Q22cf12CfObjectMoveFv, (void*)CfObject_setMoveTargetPtr__Q22cf12CfObjectMoveFv, (void*)CfObject_createMoveTarget__Q22cf12CfObjectMoveFv, (void*)CfObject_getCurrentTarget__Q22cf12CfObjectMoveFv, // +0x480
    (void*)CfObject_forwardNpcAction__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc50__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc51__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc52__Q22cf13CfObjectModelFv, // +0x490
    (void*)CfObject_UnkVirtualFunc53__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc54__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc55__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc56__Q22cf13CfObjectModelFv, // +0x4A0
    (void*)CfObject_UnkVirtualFunc57__Q22cf12CfObjectMoveFv, (void*)CfObject_getMoveRateScale__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc59__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc60__Q22cf12CfObjectMoveFv, // +0x4B0
    (void*)CfObject_setAnimSlotEntry__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc62__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc63__Q22cf13CfObjectModelFv, (void*)CfObject_setMoveBusyState__Q22cf12CfObjectMoveFi, // +0x4C0
    (void*)CfObject_setMoveReadyFlag__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc66__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc67__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFv, // +0x4D0
    (void*)CfObject_UnkVirtualFunc69__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc70__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc71__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc72__Q22cf13CfObjectModelFv, // +0x4E0
    (void*)CfObject_UnkVirtualFunc73__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc1__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc2__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc3__Q22cf13CfObjectModelFv, // +0x4F0
    (void*)CfObjectModel_UnkVirtualFunc4__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc5__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc6__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc7__Q22cf13CfObjectModelFv, // +0x500
    (void*)CfObjectModel_UnkVirtualFunc8__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc9__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc10__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc11__Q22cf13CfObjectModelFv, // +0x510
    (void*)CfObjectModel_UnkVirtualFunc12__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc13__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc14__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc15__Q22cf13CfObjectModelFv, // +0x520
    (void*)CfObjectModel_UnkVirtualFunc16__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc17__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc18__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc19__Q22cf12CfObjectMoveFv, // +0x530
    (void*)CfObjectModel_UnkVirtualFunc20__Q22cf13CfObjectModelFv, (void*)CfObjectMove_dropMoveRequest__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_detachModelList__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_releaseSlotById__Q22cf12CfObjectMoveFv, // +0x540
    (void*)CfObjectMove_recordMoveValue__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_getMovementRate__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_storeActionSrcId__Q22cf13CfObjectActorFv, (void*)CfObjectMove_refreshNpcState__Q22cf12CfObjectMoveFv, // +0x550
    (void*)CfObjectMove_updateNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_queryNpcAdvance__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_UnkVirtualFunc10__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_writeTargetField__Q22cf12CfObjectMoveFv, // +0x560
    (void*)CfObjectMove_advanceNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_commandNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_restartNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_hasActorScale__Q22cf13CfObjectActorFv, // +0x570
    (void*)CfObjectMove_attachEffectSlot__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_setEffectSlotBit__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_transferSlotBits__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_isEffectSlotUsed__Q22cf12CfObjectMoveFv, // +0x580
    (void*)CfObjectMove_signalEffectSlot__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_flushEffectSlots__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_setAllEffectBits__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_loadResourceById__Q22cf12CfObjectMoveFv, // +0x590
    (void*)__dt__Q22cf13CfObjectActorFv, (void*)CfObjectActor_hasActorScale__Q22cf13CfObjectActorFv, (void*)CfObjectActor_clearStatusPair__Q22cf13CfObjectActorFv, (void*)CfObjectActor_flushStatusPair__Q22cf13CfObjectActorFv, // +0x5A0
    (void*)CfObjectActor_takeDamageValue__Q22cf13CfObjectActorFv, (void*)CfObjectActor_readFacingAngle__Q22cf13CfObjectActorFv, (void*)CfObjectActor_getAdjustedFacing__Q22cf13CfObjectActorFv, (void*)CfObjectActor_readActionScale__Q22cf13CfObjectActorFv, // +0x5B0
    (void*)CfObjectActor_sharesMoveFlags__Q22cf13CfObjectActorFv, (void*)CfObjectActor_pushRefreshValue__Q22cf13CfObjectActorFv, (void*)CfObjectActor_storeActionSrcId__Q22cf13CfObjectActorFv, (void*)CfObjectActor_getActionSrcId__Q22cf13CfObjectActorFv, // +0x5C0
    (void*)CfObjectActor_clearActionSrcId__Q22cf13CfObjectActorFv, // +0x5D0
};

// .data 0xC: RTTI descriptor pair + pad.
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80531670[3] = {
    (void*)lbl_eu_80662400,
    0, 0
};

// .data 0x44: this TU's dispatch table (own-TU funcs + 3 cross-TU).
__declspec(section ".data") __attribute__((used))
const void* lbl_eu_8053167C[17] = {
    (void*)lbl_eu_80662408, 0, (void*)__ct__80172668, (void*)func_801728F8, // +0x0
    (void*)releaseEvtAnim28, (void*)getField20, (void*)isEvtFlagBit4Set, (void*)isEvtFlagBit0Set, // +0x10
    (void*)CREvtObjIsBusyDefault, (void*)func_801727DC, (void*)zeroEvtModelHook, (void*)CREvtObjVfunc24Default, // +0x20
    (void*)func_80172CC4, (void*)func_801729F0, (void*)emptyEvtModelHook, (void*)func_801726DC, // +0x30
    (void*)func_80172768, // +0x40
};
} // extern "C"

// (rodata struct lives at top of file; sdata pairs below.)
extern "C" char lbl_eu_80503300[];
extern "C" char lbl_eu_80503314[];
extern "C" char lbl_eu_80503324[];

// typified split1 retail .sdata: 4 RTTI-ish pointer pairs (MES pattern).
// 80503230 lives in a neighbor unit; 80531670/805316C0 are this TU's .data
// (opaque absorb kept below), so those two stay UNDEF reads for now.
extern "C" char lbl_eu_80503230[];
extern "C" void* lbl_eu_80531670[];
extern "C" void* lbl_eu_805316C0[];
__declspec(section ".sdata") __attribute__((aligned(8))) __attribute__((used)) const void* lbl_eu_806623F0[2] = {
    lbl_eu_80503230, 0
};
__declspec(section ".sdata") __attribute__((aligned(8))) __attribute__((used)) const void* lbl_eu_806623F8[2] = {
    lbl_eu_80503300, lbl_eu_80531670
};
__declspec(section ".sdata") __attribute__((aligned(8))) __attribute__((used)) const void* lbl_eu_80662400[2] = {
    lbl_eu_80503314, 0
};
__declspec(section ".sdata") __attribute__((aligned(8))) __attribute__((used)) const void* lbl_eu_80662408[2] = {
    lbl_eu_80503324, lbl_eu_805316C0
};
__declspec(section ".sdata2") __attribute__((aligned(8))) const float lbl_eu_8066777C __attribute__((used)) = 255.0f;
